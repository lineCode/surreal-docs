/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include <gtest/gtest.h>
#include <queue>
#include <iostream>
#include <utility>
#include "udocs-processor/serializers/markdown/SurrealMarkdown.h"

#define DO_PRINT_DEBUG true

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

using PVT = udocs_processor::SurrealMarkdown::ParametersValues;

struct Value {
  Value(udocs_processor::SurrealBlockParameter::Type Type, std::any Val)
      : Type(Type), Val(std::move(Val)) {}

  udocs_processor::SurrealBlockParameter::Type Type;
  std::any Val;
};

std::string& TrimAllWhitespaces(std::string& Input) {
  Input.erase(0, Input.find_first_not_of(" \n\t"));
  Input.erase(Input.find_last_not_of(" \n\t") + 1);

  return Input;
}

using PT = udocs_processor::SurrealBlockParameter::Type;

bool operator==(PVT const &A, std::map<std::string, Value> const &B) {
  using T = udocs_processor::SurrealBlockParameter::Type;
  // No predicate needed because there is operator== for pairs already.
  if (A.size() != B.size()) {
    return false;
  }

  for (const auto& Entry : A) {
    auto It = B.find(Entry.first);
    if (It != B.end()) {
      if (DO_PRINT_DEBUG) {
        std::cerr << Entry.second.type().name() << std::endl;
        std::cerr << It->second.Val.type().name() << std::endl;
      }
      if (Entry.second.type() != It->second.Val.type()) {
        return false;
      }

      switch (It->second.Type) {
        case T::STRING:
          if (std::any_cast<std::string>(Entry.second) !=
              std::any_cast<std::string>(It->second.Val)) {
            return false;
          }
          break;
        case T::INTEGER:
          if (std::any_cast<int64_t>(Entry.second) !=
              std::any_cast<int64_t>(It->second.Val)) {
            return false;
          }
          break;
        case T::BOOL:
          if (std::any_cast<bool>(Entry.second) !=
              std::any_cast<bool>(It->second.Val)) {
            return false;
          }
          break;
      }
    } else {
      return false;
    }
  }

  return true;
}

class MarkdownTester : public udocs_processor::SurrealMarkdown {
 public:
  // !!<name> - refuse, &&<name> - enter
  bool DoEnterBlock(const udocs_processor::SurrealBlock &Block,
                    ParametersValues Arguments) override {
    bool DoEnter = SurrealMarkdown::DoEnterBlock(Block, Arguments);

    if (DO_PRINT_DEBUG) {
      std::cerr << "Queue: " << QueueOfValues.size() << std::endl;
    }

    bool QueueOfValuesNotEmpty = !QueueOfValues.empty();
    bool QueueOfParamsNotEmpty = !QueueOfParamSets.empty();
    EXPECT_TRUE(QueueOfValuesNotEmpty);
    EXPECT_TRUE(QueueOfParamsNotEmpty);

    if (!QueueOfParamsNotEmpty || !QueueOfValuesNotEmpty) {
      return DoEnter;
    }

    if (DoEnter) {
      EXPECT_EQ(QueueOfValues.front(), "&&" + Block.GetName());
    } else {
      EXPECT_EQ(QueueOfValues.front(), "!!" + Block.GetName());
    }

    QueueOfValues.pop();
    QueueOfParamSets.pop();

    return DoEnter;
  }

  // <name> - enter
  void OnBlockEnter(const udocs_processor::SurrealBlock &Block,
                    ParametersValues Arguments) override {
    SurrealMarkdown::OnBlockEnter(Block, Arguments);

    ASSERT_TRUE(!QueueOfValues.empty());
    ASSERT_TRUE(!QueueOfParamSets.empty());

    if (DO_PRINT_DEBUG) {
      std::cerr << "Queue: " << QueueOfValues.size() << std::endl;
    }

    ASSERT_EQ(QueueOfValues.front(), Block.GetName());

    ASSERT_TRUE(Arguments == QueueOfParamSets.front());

    QueueOfValues.pop();
    QueueOfParamSets.pop();
  }

  // !<name> - exit
  void OnBlockExit(const udocs_processor::SurrealBlock &Block,
                   ParametersValues Arguments) override {
    SurrealMarkdown::OnBlockExit(Block, Arguments);

    if (DO_PRINT_DEBUG) {
      std::cerr << "Queue: " << QueueOfValues.size() << std::endl;
    }

    ASSERT_TRUE(!QueueOfValues.empty());
    ASSERT_TRUE(!QueueOfParamSets.empty());
    ASSERT_EQ(QueueOfValues.front(), "!" + Block.GetName());

    ASSERT_TRUE(Arguments == QueueOfParamSets.front());

    QueueOfValues.pop();
    QueueOfParamSets.pop();
  }

  // +<text> - text
  void OnMarkdown(const std::string &Markdown) override {
    SurrealMarkdown::OnMarkdown(Markdown);

    if (!DoAcceptMarkdown()) {
      return;
    }

    if (DO_PRINT_DEBUG) {
      std::cerr << "Queue: " << QueueOfValues.size() << std::endl;
    }

    ASSERT_TRUE(!QueueOfValues.empty());
    ASSERT_EQ(QueueOfValues.front(), "+" + Markdown);

    QueueOfValues.pop();
  }

  void OnEOF() override {
    ASSERT_TRUE(QueueOfValues.empty());
    ASSERT_TRUE(QueueOfParamSets.empty());

    EofHit = true;
  }

  void Parse(const std::string &SurrealMarkdown) override {
    SurrealMarkdown::Parse(SurrealMarkdown);

    ASSERT_TRUE(EofHit);
  }

  std::queue<std::string> QueueOfValues;
  std::queue<std::map<std::string, Value>> QueueOfParamSets;

  bool EofHit = false;
};

TEST(SurrealMarkdownBlocks, SingleBlockWithNewlinesAtStartAndEnd) {
    std::string Input = R"(
``tabs
``
)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&tabs");
  Tester.QueueOfValues.push("tabs");
  Tester.QueueOfValues.push("!tabs");

  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});

  Tester.Parse(Input);
}

TEST(SurrealMarkdownBlocks, SingleBlock2NoNewlineAtStart) {
  std::string Input = R"(``tabs
``
)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&tabs");
  Tester.QueueOfValues.push("tabs");
  Tester.QueueOfValues.push("!tabs");

  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});

  Tester.Parse(Input);
}

TEST(SurrealMarkdownBlocks, SingleBlock2NoNewlineAtEnd) {
  std::string Input = R"(
``tabs
``)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&tabs");
  Tester.QueueOfValues.push("tabs");
  Tester.QueueOfValues.push("!tabs");

  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});

  Tester.Parse(Input);
}

TEST(SurrealMarkdownBlocks, SingleBlock2ManyNewlinesAtStartAndEndAndInMiddle) {
  std::string Input = R"(

    )" R"(



``tabs

      )" R"(


``
   )" R"(

)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&tabs");
  Tester.QueueOfValues.push("tabs");
  Tester.QueueOfValues.push("!tabs");

  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});

  Tester.Parse(Input);
}

TEST(SurrealMarkdownBlocks, SingleBlockWithTexts) {
  std::string Input = R"(
text1
``warning
text2
``
text3)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("+text1\n\n");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("+\n\ntext2\n\n");
  Tester.QueueOfValues.push("!warning");
  Tester.QueueOfValues.push("+\n\ntext3");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownBlocks, SingleBlockWithTexts2) {
  std::string Input = R"(
text1
``warning
text2

text3
``
text3)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("+text1\n\n");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("+\n\ntext2\n\ntext3\n\n");
  Tester.QueueOfValues.push("!warning");
  Tester.QueueOfValues.push("+\n\ntext3");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownBlocks, SingleBlockWhitespacesAfterName) {
  std::string Input = R"(
text1
)"
                      R"(``warning    )"
                      R"(
text2

text3
``
text3)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("+text1\n\n");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("+\n\ntext2\n\ntext3\n\n");
  Tester.QueueOfValues.push("!warning");
  Tester.QueueOfValues.push("+\n\ntext3");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownBlocks, SingleBlockWhitespacesBeforeBlockStart) {
  std::string Input = R"(
text1
)"
                      R"(   ``warning)"
                      R"(
text2

text3
``
text3)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("+text1\n\n");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("+\n\ntext2\n\ntext3\n\n");
  Tester.QueueOfValues.push("!warning");
  Tester.QueueOfValues.push("+\n\ntext3");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownBlocks, SingleBlockWhitespacesBeforeName) {
  std::string Input = R"(
text1
)"
                      R"(``   warning)"
                      R"(
text2

text3
``
text3)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("+text1\n\n");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("+\n\ntext2\n\ntext3\n\n");
  Tester.QueueOfValues.push("!warning");
  Tester.QueueOfValues.push("+\n\ntext3");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownBlocks, SingleBlockWhitespacesInStart) {
  std::string Input = R"(
text1
)"
                      R"(    ``   warning    )"
                      R"(
text2

text3
``
text3)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("+text1\n\n");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("+\n\ntext2\n\ntext3\n\n");
  Tester.QueueOfValues.push("!warning");
  Tester.QueueOfValues.push("+\n\ntext3");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownBlocks, SingleBlockMultilines) {
  std::string Input = R"(
text1


text2
``warning
text2
text3
``
text3


text4)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("+text1\n\n\ntext2\n\n");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("+\n\ntext2\ntext3\n\n");
  Tester.QueueOfValues.push("!warning");
  Tester.QueueOfValues.push("+\n\ntext3\n\n\ntext4");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownBlocks, SingleBlockEmptyName) {
  std::string Input = R"(
text2
``
text2
``
text4)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("+text2\n``\ntext2\n``\ntext4");

  Tester.Parse(Input);
}

TEST(SurrealMarkdownBlocks, SingleLine) {
  std::string Input = R"(``warning)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("warning");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownBlocks, SingleBlockUnregisteredName) {
  std::string Input = R"(
text1
``tab
text2
``)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("+text1\n``tab\ntext2\n``");

  Tester.Parse(Input);
}

TEST(SurrealMarkdownBlocks, SingleBlockUnclosed) {
  std::string Input = R"(
text1
``warning
text2
`)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("+text1\n\n");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("+\n\ntext2\n`");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownBlocks, SingleBlockUnclosed2) {
  std::string Input = R"(
text1
``warning
text2`)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("+text1\n\n");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("+\n\ntext2`");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownBlocks, SingleBlockSymbolsAfterName) {
  std::string Input = R"(
``tabs `
``
)";

  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("+``tabs `\n``");

  Tester.Parse(Input);
}

TEST(SurrealMarkdownBlocks, SingleBlockSpacesBeforeEnd) {
  std::string Input = R"(
text1
``warning
text2
   ``)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("+text1\n\n");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("+\n\ntext2\n\n");
  Tester.QueueOfValues.push("!warning");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownBlocks, SingleBlockSpacesAfterEnd) {
  std::string Input = R"(
text1
``warning
text2
   ``   )";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("+text1\n\n");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("+\n\ntext2\n\n");
  Tester.QueueOfValues.push("!warning");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownBlocks, SingleBlockSymbolsAfterEnd) {
  std::string Input = R"(
text1
``warning
text2
   `` `)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("+text1\n\n");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("+\n\ntext2\n   `` `");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownBlocks, NestedBlocks) {
  std::string Input = R"(``warning
``warning
``warning
``warning
text2
``
text3
``
text4
``warning
``warning
``
``
``
``)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("+\n\ntext2\n\n");
  Tester.QueueOfValues.push("!warning");
  Tester.QueueOfValues.push("+\n\ntext3\n\n");
  Tester.QueueOfValues.push("!warning");
  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("+\n\ntext4\n\n");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("!warning");
  Tester.QueueOfValues.push("!warning");
  Tester.QueueOfValues.push("!warning");
  Tester.QueueOfValues.push("!warning");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, SingleBlockParameterString) {
  std::string Input = R"(
``warning
title: title
text2
``)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("+\n\ntext2\n\n");
  Tester.QueueOfValues.push("!warning");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("title")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, SingleBlockParameterBool) {
  std::string Input = R"(
``warning1
title: false
text2
``)";
  MarkdownTester Tester;
  Tester.Initialize();
  udocs_processor::SurrealBlock Warning1;
  Warning1.SetName("warning1");
  Warning1.AddParameter("title", PT::BOOL,
      false, false);
  Tester.AddSupportedBlock(Warning1);

  Tester.QueueOfValues.push("&&warning1");
  Tester.QueueOfValues.push("warning1");
  Tester.QueueOfValues.push("+\n\ntext2\n\n");
  Tester.QueueOfValues.push("!warning1");

  std::map<std::string, Value> Args = {
      {"title", {PT::BOOL, false}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, SingleBlockParameterBool2) {
  std::string Input =
      R"(``warning1
title: falsE
text2
``)";
  MarkdownTester Tester;
  Tester.Initialize();
  udocs_processor::SurrealBlock Warning1;
  Warning1.SetName("warning1");
  Warning1.AddParameter("title", PT::BOOL,
      true, false);
  Tester.AddSupportedBlock(Warning1);

  Tester.QueueOfValues.push("+" + Input);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, SingleBlockParameterBool3) {
  std::string Input = R"(
``warning1
title: true
text2
``)";
  MarkdownTester Tester;
  Tester.Initialize();
  udocs_processor::SurrealBlock Warning1;
  Warning1.SetName("warning1");
  Warning1.AddParameter("title", PT::BOOL,
      false, false);
  Tester.AddSupportedBlock(Warning1);

  Tester.QueueOfValues.push("&&warning1");
  Tester.QueueOfValues.push("warning1");
  Tester.QueueOfValues.push("+\n\ntext2\n\n");
  Tester.QueueOfValues.push("!warning1");

  std::map<std::string, Value> Args = {
      {"title", {PT::BOOL, true}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, SingleBlockParameterTrim) {
  std::string Input = R"(
``warning1
title: true   )" R"(
text2
``)";
  MarkdownTester Tester;
  Tester.Initialize();
  udocs_processor::SurrealBlock Warning1;
  Warning1.SetName("warning1");
  Warning1.AddParameter("title", PT::BOOL,
      false, false);
  Tester.AddSupportedBlock(Warning1);

  Tester.QueueOfValues.push("&&warning1");
  Tester.QueueOfValues.push("warning1");
  Tester.QueueOfValues.push("+\n\ntext2\n\n");
  Tester.QueueOfValues.push("!warning1");

  std::map<std::string, Value> Args = {
      {"title", {PT::BOOL, true}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, SingleBlockParameterSymbolsAfterValue) {
  std::string Input = R"(``warning1
title: true   `)" R"(
text2
``)";
  MarkdownTester Tester;
  Tester.Initialize();
  udocs_processor::SurrealBlock Warning1;
  Warning1.SetName("warning1");
  Warning1.AddParameter("title", PT::BOOL,
      false, false);
  Tester.AddSupportedBlock(Warning1);

  Tester.QueueOfValues.push("+" + Input);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, SingleBlockParameterInt64) {
  std::string Input = R"(
``warning1
title: 12
text2
``)";
  MarkdownTester Tester;
  Tester.Initialize();
  udocs_processor::SurrealBlock Warning1;
  Warning1.SetName("warning1");
  Warning1.AddParameter("title", PT::INTEGER,
      false, 1);
  Tester.AddSupportedBlock(Warning1);

  Tester.QueueOfValues.push("&&warning1");
  Tester.QueueOfValues.push("warning1");
  Tester.QueueOfValues.push("+\n\ntext2\n\n");
  Tester.QueueOfValues.push("!warning1");

  std::map<std::string, Value> Args = {
      {"title", {PT::INTEGER, static_cast<int64_t>(12)}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, SingleBlockParameterOptionalInt) {
  std::string Input = R"(
``warning1
text2
``)";
  MarkdownTester Tester;
  Tester.Initialize();
  udocs_processor::SurrealBlock Warning1;
  Warning1.SetName("warning1");
  Warning1.AddParameter("title", PT::INTEGER,
      true, static_cast<int64_t>(111));
  Tester.AddSupportedBlock(Warning1);

  Tester.QueueOfValues.push("&&warning1");
  Tester.QueueOfValues.push("warning1");
  Tester.QueueOfValues.push("+\n\ntext2\n\n");
  Tester.QueueOfValues.push("!warning1");

  std::map<std::string, Value> Args = {
      {"title", {PT::INTEGER, static_cast<int64_t>(111)}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, SingleBlockParameterOptionalBool) {
  std::string Input = R"(
``warning1
text2
``)";
  MarkdownTester Tester;
  Tester.Initialize();
  udocs_processor::SurrealBlock Warning1;
  Warning1.SetName("warning1");
  Warning1.AddParameter("title", PT::BOOL,
      true, true);
  Tester.AddSupportedBlock(Warning1);

  Tester.QueueOfValues.push("&&warning1");
  Tester.QueueOfValues.push("warning1");
  Tester.QueueOfValues.push("+\n\ntext2\n\n");
  Tester.QueueOfValues.push("!warning1");

  std::map<std::string, Value> Args = {
      {"title", {PT::BOOL, true}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, SingleBlockParameterOptionalString) {
  std::string Input = R"(
``warning1
text2
``)";
  MarkdownTester Tester;
  Tester.Initialize();
  udocs_processor::SurrealBlock Warning1;
  Warning1.SetName("warning1");
  Warning1.AddParameter("title", PT::STRING,
      true, std::string("11111"));
  Tester.AddSupportedBlock(Warning1);

  Tester.QueueOfValues.push("&&warning1");
  Tester.QueueOfValues.push("warning1");
  Tester.QueueOfValues.push("+\n\ntext2\n\n");
  Tester.QueueOfValues.push("!warning1");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("11111")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, SingleBlockParameterNotOptional) {
  std::string Input = R"(``warning1)" R"(
text2
``)";
  MarkdownTester Tester;
  Tester.Initialize();
  udocs_processor::SurrealBlock Warning1;
  Warning1.SetName("warning1");
  Warning1.AddParameter("title", PT::STRING,
      false, "");
  Tester.AddSupportedBlock(Warning1);

  Tester.QueueOfValues.push("+" + Input);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, SingleBlockParameterExtra) {
  std::string Input = R"(``warning1
title: 1
param2: 2
text2
``)";
  MarkdownTester Tester;
  Tester.Initialize();
  udocs_processor::SurrealBlock Warning1;
  Warning1.SetName("warning1");
  Warning1.AddParameter("title", PT::STRING,
      false, std::string("11111"));
  Tester.AddSupportedBlock(Warning1);

  Tester.QueueOfValues.push("+" + Input);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, SingleBlockSeveralParameters) {
  std::string Input = R"(``warning1
title: 1
title2: 2
text2
``)";
  MarkdownTester Tester;
  Tester.Initialize();
  udocs_processor::SurrealBlock Warning1;
  Warning1.SetName("warning1");
  Warning1.AddParameter("title", PT::STRING,
      false, std::string("11111"));
  Warning1.AddParameter("title2", PT::STRING,
      false, std::string("111121"));
  Tester.AddSupportedBlock(Warning1);

  Tester.QueueOfValues.push("&&warning1");
  Tester.QueueOfValues.push("warning1");
  Tester.QueueOfValues.push("+\n\ntext2\n\n");
  Tester.QueueOfValues.push("!warning1");

  std::map<std::string, Value> Args = {
      {"title2", {PT::STRING, std::string("2")}},
      {"title", {PT::STRING, std::string("1")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, SingleBlockSeveralOptionalParameters) {
  std::string Input = R"(``warning1
text2
``)";
  MarkdownTester Tester;
  Tester.Initialize();
  udocs_processor::SurrealBlock Warning1;
  Warning1.SetName("warning1");
  Warning1.AddParameter("title", PT::STRING,
      true, std::string("11111"));
  Warning1.AddParameter("title2", PT::STRING,
      true, std::string("111121"));
  Tester.AddSupportedBlock(Warning1);

  Tester.QueueOfValues.push("&&warning1");
  Tester.QueueOfValues.push("warning1");
  Tester.QueueOfValues.push("+\n\ntext2\n\n");
  Tester.QueueOfValues.push("!warning1");

  std::map<std::string, Value> Args = {
      {"title2", {PT::STRING, std::string("111121")}},
      {"title", {PT::STRING, std::string("11111")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(StandardSurrealMarkdown, WarningTitleDefaultValue) {
  std::string Input = R"(``warning
text2
``)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("+\n\ntext2\n\n");
  Tester.QueueOfValues.push("!warning");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(StandardSurrealMarkdown, SpoilerTitleDefaultValue) {
  std::string Input = R"(``spoiler
text2
``)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&spoiler");
  Tester.QueueOfValues.push("spoiler");
  Tester.QueueOfValues.push("+\n\ntext2\n\n");
  Tester.QueueOfValues.push("!spoiler");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}},
      {"opened", {PT::BOOL, false}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(StandardSurrealMarkdown, FigureTitleDefaultValue) {
  std::string Input = R"(``figure
text2
``)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&figure");
  Tester.QueueOfValues.push("figure");
  Tester.QueueOfValues.push("+\n\ntext2\n\n");
  Tester.QueueOfValues.push("!figure");

  std::map<std::string, Value> Args = {
      {"url", {PT::STRING, std::string("")}},
      {"align", {PT::STRING, std::string("default")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(StandardSurrealMarkdown, PageTitleDefaultValue) {
  std::string Input = R"(``tabs
``page
text2
``
``)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&tabs");
  Tester.QueueOfValues.push("tabs");
  Tester.QueueOfValues.push("&&page");
  Tester.QueueOfValues.push("page");
  Tester.QueueOfValues.push("+\n\ntext2\n\n");
  Tester.QueueOfValues.push("!page");
  Tester.QueueOfValues.push("!tabs");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push({});

  Tester.Parse(Input);
}

TEST(StandardSurrealMarkdown, RejectionOfTextDirectlyInsideTabs) {
  std::string Input = R"(``tabs
text24
``page
text2
``
text23
``)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&tabs");
  Tester.QueueOfValues.push("tabs");
  Tester.QueueOfValues.push("&&page");  // expecting text24 to be rejected
  Tester.QueueOfValues.push("page");
  Tester.QueueOfValues.push("+\n\ntext2\n\n");
  Tester.QueueOfValues.push("!page");  // expecting text23 to be rejected
  Tester.QueueOfValues.push("!tabs");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push({});

  Tester.Parse(Input);
}

TEST(StandardSurrealMarkdown, RejectionOfBlockStartDirectlyInsideTabs) {
  std::string Input = R"(``tabs
text24
``page
text2
``
``warning
text23
``)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&tabs");
  Tester.QueueOfValues.push("tabs");
  Tester.QueueOfValues.push("&&page");  // expecting text24 to be rejected
  Tester.QueueOfValues.push("page");
  Tester.QueueOfValues.push("+\n\ntext2\n\n");
  Tester.QueueOfValues.push("!page");  // expecting text23 to be rejected
  Tester.QueueOfValues.push("!!warning");  // expecting warning to be rejected
  Tester.QueueOfValues.push("!tabs");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push({});

  Tester.Parse(Input);
}

TEST(StandardSurrealMarkdown, RejectionOfBlockStartWithEndDirectlyInsideTabs) {
  std::string Input = R"(``tabs
text24
``page
text2
``
``warning
text23
``
``)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&tabs");
  Tester.QueueOfValues.push("tabs");
  Tester.QueueOfValues.push("&&page");  // expecting text24 to be rejected
  Tester.QueueOfValues.push("page");
  Tester.QueueOfValues.push("+\n\ntext2\n\n");
  Tester.QueueOfValues.push("!page");  // expecting text23 to be rejected
  Tester.QueueOfValues.push("!!warning");  // expecting warning to be rejected
  Tester.QueueOfValues.push("!tabs");
  Tester.QueueOfValues.push("+\n\n``");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push({});

  Tester.Parse(Input);
}

TEST(StandardSurrealMarkdown, PageOutsideOfTabs) {
  std::string Input = R"(``page
text2
``)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("!!page");
  Tester.QueueOfValues.push("+" + Input);

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(StandardSurrealMarkdown, PageInsideOfTabsOfTabs) {
  std::string Input = R"(``tabs
``page
``tabs
``page
text2
``
``
``
``)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&tabs");
  Tester.QueueOfValues.push("tabs");
  Tester.QueueOfValues.push("&&page");
  Tester.QueueOfValues.push("page");
  Tester.QueueOfValues.push("&&tabs");
  Tester.QueueOfValues.push("tabs");
  Tester.QueueOfValues.push("&&page");
  Tester.QueueOfValues.push("page");
  Tester.QueueOfValues.push("+\n\ntext2\n\n");
  Tester.QueueOfValues.push("!page");
  Tester.QueueOfValues.push("!tabs");
  Tester.QueueOfValues.push("!page");
  Tester.QueueOfValues.push("!tabs");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push({});

  Tester.Parse(Input);
}

TEST(StandardSurrealMarkdown, PageInsideOfTabsOfTabsWith2Tabs) {
  std::string Input = R"(``tabs
``page
``tabs
``page
text2
``
``
``
``page
text
``
``)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&tabs");
  Tester.QueueOfValues.push("tabs");
  Tester.QueueOfValues.push("&&page");
  Tester.QueueOfValues.push("page");
  Tester.QueueOfValues.push("&&tabs");
  Tester.QueueOfValues.push("tabs");
  Tester.QueueOfValues.push("&&page");
  Tester.QueueOfValues.push("page");
  Tester.QueueOfValues.push("+\n\ntext2\n\n");
  Tester.QueueOfValues.push("!page");
  Tester.QueueOfValues.push("!tabs");
  Tester.QueueOfValues.push("!page");
  Tester.QueueOfValues.push("&&page");
  Tester.QueueOfValues.push("page");
  Tester.QueueOfValues.push("+\n\ntext\n\n");
  Tester.QueueOfValues.push("!page");
  Tester.QueueOfValues.push("!tabs");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push({});

  Tester.Parse(Input);
}

TEST(StandardSurrealMarkdown, PageAfterTabs) {
  std::string Input = R"(``tabs
``
``page
text2
``)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&tabs");
  Tester.QueueOfValues.push("tabs");
  Tester.QueueOfValues.push("!tabs");
  Tester.QueueOfValues.push("!!page");
  Tester.QueueOfValues.push("+\n\n``page\ntext2\n``");
  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, Unclosed) {
  std::string Input = R"(``warning
title: 11)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("warning");
  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("11")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, SpacesBeforeName) {
  std::string Input = R"(``warning
    title: 11
)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("warning");
  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("11")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, SpacesAfterName) {
  std::string Input = R"(``warning
title     : 11
)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("warning");
  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("11")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, SpacesAroundName) {
  std::string Input = R"(``warning
      title     : 11
)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("warning");
  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("11")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, DuplicateParameter) {
  std::string Input = R"(``warning
      title     : 11
title:11
)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("+" + TrimAllWhitespaces(Input));

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, NewlineBetweenParameters) {
  std::string Input = R"(``spoiler
      title     : 111

opened: true
)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&spoiler");
  Tester.QueueOfValues.push("spoiler");
  Tester.QueueOfValues.push("+\n\nopened: true");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("111")}},
      {"opened", {PT::BOOL, false}}  // default val.
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, TextBetweenParameters) {
  std::string Input = R"(``spoiler
      title     : 111
33
opened: true
)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&spoiler");
  Tester.QueueOfValues.push("spoiler");
  Tester.QueueOfValues.push("+\n\n33\nopened: true");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("111")}},
      {"opened", {PT::BOOL, false}}  // default val.
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, BlockEndParameters) {
  std::string Input = R"(``spoiler
      title     : 111
``
opened: true
)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&spoiler");
  Tester.QueueOfValues.push("spoiler");
  Tester.QueueOfValues.push("!spoiler");
  Tester.QueueOfValues.push("+\n\nopened: true");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("111")}},
      {"opened", {PT::BOOL, false}}  // default val.
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, NoSpaces) {
  std::string Input = R"(``warning
title:11
)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("warning");
  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("11")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, EmptyName) {
  std::string Input = R"(``warning
:11
)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("+\n\n:11");
  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, EmptyValue) {
  std::string Input = R"(``warning
title:
)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("warning");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownCode, AllCode) {
  std::string Input = R"(```
``warning
      title     : 11
```)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("+" + Input);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownCode, AllCode2) {
  std::string Input = R"(````
``warning
      title     : 11
```)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("+" + Input);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownCode, AllCode3) {
  std::string Input = R"(```    `adasdasd
``warning
      title     : 11
```)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("+" + Input);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownCode, AllCodeUnclosed) {
  std::string Input = R"(```
``warning
      title     : 11
)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("+" + TrimAllWhitespaces(Input));

  Tester.Parse(Input);
}

TEST(SurrealMarkdownCode, AllCodeNewlineAtTheEnd) {
  std::string Input = R"(```
``warning
      title     : 11
```
)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("+" + TrimAllWhitespaces(Input));

  Tester.Parse(Input);
}

TEST(SurrealMarkdownCode, AllCodeNewlineAtTheStart) {
  std::string Input = R"(
```
``warning
      title     : 11
```)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("+" + TrimAllWhitespaces(Input));

  Tester.Parse(Input);
}

TEST(SurrealMarkdownCode, AllCodeNewlineStartEnd) {
  std::string Input = R"(
```
``warning
      title     : 11
```
)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("+" + TrimAllWhitespaces(Input));

  Tester.Parse(Input);
}

TEST(SurrealMarkdownCode, TwoCodeBlocks) {
  std::string Input = R"(
```
``warning
      title     : 11
```
```
ooo
```
)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("+" + TrimAllWhitespaces(Input));

  Tester.Parse(Input);
}

TEST(SurrealMarkdownCode, TwoCodeBlocksWithBlockBetween) {
  std::string Input = R"(
```
``warning
      title     : 11
```
``warning
```
ooo
```
)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("+```\n``warning\n      title     : 11\n```\n\n");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("+\n\n```\nooo\n```");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownCode, BlockThenCode) {
  std::string Input = R"(
``warning1
```
title     : 11
```
)";
  MarkdownTester Tester;
  Tester.Initialize();

  udocs_processor::SurrealBlock Warning1;
  Warning1.SetName("warning1");
  Tester.AddSupportedBlock(Warning1);

  Tester.QueueOfValues.push("&&warning1");
  Tester.QueueOfValues.push("warning1");
  Tester.QueueOfValues.push("+\n\n```\ntitle     : 11\n```");

  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});

  Tester.Parse(Input);
}

TEST(SurrealMarkdownCode, BlockThenCodeTrailingSpaces) {
  std::string Input = R"(
``warning1
```   )" R"(
title     : 11
```   )" R"(
)";
  MarkdownTester Tester;
  Tester.Initialize();

  udocs_processor::SurrealBlock Warning1;
  Warning1.SetName("warning1");
  Tester.AddSupportedBlock(Warning1);

  Tester.QueueOfValues.push("&&warning1");
  Tester.QueueOfValues.push("warning1");
  Tester.QueueOfValues.push("+\n\n```   \ntitle     : 11\n```");

  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});

  Tester.Parse(Input);
}

TEST(SurrealMarkdownCode, BlockThenCodeStartingSpaces) {
  std::string Input = R"(
``warning1
    ```
title     : 11
    ```
)";
  MarkdownTester Tester;
  Tester.Initialize();

  udocs_processor::SurrealBlock Warning1;
  Warning1.SetName("warning1");
  Tester.AddSupportedBlock(Warning1);

  Tester.QueueOfValues.push("&&warning1");
  Tester.QueueOfValues.push("warning1");
  Tester.QueueOfValues.push("+\n\n```\ntitle     : 11\n    ```");

  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});

  Tester.Parse(Input);
}

TEST(SurrealMarkdownCode, BlockThenCodeTrailingSymbols) {
  std::string Input = R"(
``warning1
```asdadad  asdas
title     : 11
    ```
)";
  MarkdownTester Tester;
  Tester.Initialize();

  udocs_processor::SurrealBlock Warning1;
  Warning1.SetName("warning1");
  Tester.AddSupportedBlock(Warning1);

  Tester.QueueOfValues.push("&&warning1");
  Tester.QueueOfValues.push("warning1");
  Tester.QueueOfValues.push(
      "+\n\n```asdadad  asdas\ntitle     : 11\n    ```");

  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});

  Tester.Parse(Input);
}

TEST(SurrealMarkdownCode, TildaCode) {
  std::string Input = R"(
~~~
``warning
      title     : 11
~~~)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("+" + TrimAllWhitespaces(Input));

  Tester.Parse(Input);
}

TEST(SurrealMarkdownCode, MixedCode) {
  std::string Input = R"(
~`~
``warning
~``)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("+~`~\n\n");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("+\n\n~``");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownCode, MixedCode2) {
  std::string Input = R"(
`~`~
``warning
~``)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("+`~`~\n\n");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("+\n\n~``");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownCode, MixedCode3) {
  std::string Input = R"(
``~
``warning
~~~`)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("+``~\n\n");
  Tester.QueueOfValues.push("warning");
  Tester.QueueOfValues.push("+\n\n~~~`");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

// should fail
TEST(SurrealMarkdownCode, DifferentEndAndBegin) {
  std::string Input = R"(
```
``warning
~~~

``warning)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("+" + TrimAllWhitespaces(Input));

  Tester.Parse(Input);
}

// should pass
TEST(SurrealMarkdownCode, EndOfComment) {
  std::string Input = R"(
```
``warning
```
``warning)";
  MarkdownTester Tester;
  Tester.Initialize();

  Tester.QueueOfValues.push("&&warning");
  Tester.QueueOfValues.push("+```\n``warning\n```\n\n");
  Tester.QueueOfValues.push("warning");

  std::map<std::string, Value> Args = {
      {"title", {PT::STRING, std::string("")}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownCode, BlockThenCodeSimilarToParam) {
  std::string Input = R"(
``warning1
```title: 11
title     : 11
```
)";
  MarkdownTester Tester;
  Tester.Initialize();

  udocs_processor::SurrealBlock Warning1;
  Warning1.SetName("warning1");
  Tester.AddSupportedBlock(Warning1);

  Tester.QueueOfValues.push("&&warning1");
  Tester.QueueOfValues.push("warning1");
  Tester.QueueOfValues.push("+\n\n```title: 11\ntitle     : 11\n```");

  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});

  Tester.Parse(Input);
}

TEST(SurrealMarkdownBlocks, EmptyString) {
  std::string Input;

  MarkdownTester Tester;
  Tester.Initialize();

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, SingleBlockNewlineThenNotParam) {
  std::string Input = R"(``warning1

title:title
``)";
  MarkdownTester Tester;
  Tester.Initialize();

  udocs_processor::SurrealBlock Warning1;
  Warning1.SetName("warning1");
  Tester.AddSupportedBlock(Warning1);

  Tester.QueueOfValues.push("&&warning1");
  Tester.QueueOfValues.push("warning1");
  Tester.QueueOfValues.push("+\n\ntitle:title\n\n");
  Tester.QueueOfValues.push("!warning1");

  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});

  Tester.Parse(Input);
}

TEST(SurrealMarkdownParameters, SingleBlockTextThenNotParam) {
  std::string Input = R"(``warning1
text
title:title
``)";
  MarkdownTester Tester;
  Tester.Initialize();

  udocs_processor::SurrealBlock Warning1;
  Warning1.SetName("warning1");
  Tester.AddSupportedBlock(Warning1);

  Tester.QueueOfValues.push("&&warning1");
  Tester.QueueOfValues.push("warning1");
  Tester.QueueOfValues.push("+\n\ntext\ntitle:title\n\n");
  Tester.QueueOfValues.push("!warning1");

  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push({});

  Tester.Parse(Input);
}

TEST(SurrealMarkdownContents, NoMarkdownInsideContents) {
  std::string Input = R"(``contents
text
title:title
``)";
  MarkdownTester Tester;
  Tester.Initialize();

  udocs_processor::SurrealBlock Warning1;
  Warning1.SetName("contents");
  Tester.AddSupportedBlock(Warning1);

  Tester.QueueOfValues.push("&&contents");
  Tester.QueueOfValues.push("contents");
  Tester.QueueOfValues.push("!contents");

  std::map<std::string, Value> Args = {
      {"expand", {PT::BOOL, false}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownContents, NoBlocksInsideContents) {
  std::string Input = R"(``contents
text
title:title
``warning
`1`
``)";
  MarkdownTester Tester;
  Tester.Initialize();

  udocs_processor::SurrealBlock Warning1;
  Warning1.SetName("contents");
  Tester.AddSupportedBlock(Warning1);

  Tester.QueueOfValues.push("&&contents");
  Tester.QueueOfValues.push("contents");
  Tester.QueueOfValues.push("!!warning");
  Tester.QueueOfValues.push("!contents");

  std::map<std::string, Value> Args = {
      {"expand", {PT::BOOL, false}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);
}

TEST(SurrealMarkdownContents, Contents) {
  std::string Input = R"(``contents
expand: true
text
title:title
    )" R"(
``warning
`1`


``)";
  MarkdownTester Tester;
  Tester.Initialize();

  udocs_processor::SurrealBlock Warning1;
  Warning1.SetName("contents");
  Tester.AddSupportedBlock(Warning1);

  Tester.QueueOfValues.push("&&contents");
  Tester.QueueOfValues.push("contents");
  Tester.QueueOfValues.push("!!warning");
  Tester.QueueOfValues.push("!contents");

  std::map<std::string, Value> Args = {
      {"expand", {PT::BOOL, true}}
  };

  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push(Args);
  Tester.QueueOfParamSets.push({});
  Tester.QueueOfParamSets.push(Args);

  Tester.Parse(Input);

  std::vector<std::string> D = Tester.ExtractContents();
  bool DoesContentsMatch =
      Tester.ExtractContents() == std::vector<std::string>{
          "text",
          "title:title",
          "``warning",
          "`1`"
      };
  ASSERT_TRUE(DoesContentsMatch);
}
