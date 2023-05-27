/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <vector>
#include <string>
#include <cstdint>

namespace udocs_processor {
class DynamicPageData {
 public:
  class LinkData {
   public:
    LinkData() = default;

    LinkData(std::string Title, std::string Link);

    const std::string &GetTitle() const;

    const std::string &GetLink() const;

   private:
    std::string Title;
    std::string Link;
  };
  const std::string &GetThemeAnnouncement() const;

  void SetThemeAnnouncement(std::string ThemeAnnouncement);

  const std::string &GetLatestVersion() const;

  void SetLatestVersion(std::string LatestVersion);

  const std::string &GetSurrealDocsVersion() const;

  void SetSurrealDocsVersion(std::string SurrealDocsVersion);

  const std::vector<std::string> &GetAvailableVersions() const;

  void SetAvailableVersions(std::vector<std::string> AvailableVersions);

  const std::string &GetCanonicalPage() const;

  void SetCanonicalPage(std::string CanonicalPage);

  const LinkData &GetPreviousPage() const;

  void SetPreviousPage(LinkData PreviousPage);

  const LinkData &GetNextPage() const;

  void SetNextPage(LinkData NextPage);

  int64_t GetLastUpdated() const;

  void SetLastUpdated(int64_t LastUpdated);

  const std::string &GetInstance() const;

  void SetInstance(std::string Instance);

  const std::string &GetProject() const;

  void SetProject(std::string Project);

 private:
  std::string ThemeAnnouncement;
  std::string LatestVersion;
  std::string SurrealDocsVersion;
  std::string CanonicalPage;
  std::string Instance;
  std::string Project;
  std::vector<std::string> AvailableVersions;
  LinkData PreviousPage;
  LinkData NextPage;
  int64_t LastUpdated = 0;
};
}  // namespace udocs_processor
