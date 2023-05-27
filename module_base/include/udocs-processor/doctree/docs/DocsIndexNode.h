/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <string>
#include <memory>
#include "udocs-processor/doctree/DocNode.h"

namespace udocs_processor {
class DocsIndexNode : public DocNode {
 public:
  DocsIndexNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  const std::string &GetLogoPath() const;

  void SetLogoPath(std::string LogoPath);

  const std::string &GetTag() const;

  void SetTag(std::string Tag);

  const std::string &GetVersion() const;

  void SetVersion(std::string Version);

  const std::string &GetWebSite() const;

  void SetWebSite(std::string WebSite);

  const std::string &GetOrganization() const;

  void SetOrganization(std::string Organization);

  const std::string &GetCopyright() const;

  void SetCopyright(std::string Copyright);

  const std::string &GetCopyrightUrl() const;

  void SetCopyrightUrl(std::string CopyrightUrl);

  const std::string &GetDocsTitle() const;

  void SetDocsTitle(std::string DocsTitle);

  const std::string &GetLanguage() const;

  void SetLanguage(std::string Language);

  const std::string &GetMasterPage() const;

  void SetMasterPage(std::string MasterPage);

  const std::string &GetImagesRoot() const;

  void SetImagesRoot(std::string ImagesRoot);

  int Accept(DocNodeVisitor &Visitor) override;

  ~DocsIndexNode() override = default;

 private:
  std::string LogoPath;
  std::string Tag;
  std::string Version;
  std::string WebSite;
  std::string Organization;
  std::string Copyright;
  std::string CopyrightUrl;
  std::string ImagesRoot;
  std::string DocsTitle;
  std::string Language;
  std::string MasterPage;
};
}  // namespace udocs_processor
