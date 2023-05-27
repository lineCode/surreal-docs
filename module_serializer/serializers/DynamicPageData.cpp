/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/serializers/DynamicPageData.h"

#include <utility>
#include <string>
#include <vector>

const std::string &
  udocs_processor::DynamicPageData::GetThemeAnnouncement() const {
  return ThemeAnnouncement;
}

void udocs_processor::DynamicPageData::SetThemeAnnouncement(
    std::string ThemeAnnouncement) {
  this->ThemeAnnouncement = std::move(ThemeAnnouncement);
}

const std::string &udocs_processor::DynamicPageData::GetLatestVersion() const {
  return LatestVersion;
}

void udocs_processor::DynamicPageData::SetLatestVersion(
    std::string LatestVersion) {
  this->LatestVersion = std::move(LatestVersion);
}

const std::string &
  udocs_processor::DynamicPageData::GetSurrealDocsVersion() const {
  return SurrealDocsVersion;
}

void udocs_processor::DynamicPageData::SetSurrealDocsVersion(
    std::string SurrealDocsVersion) {
  this->SurrealDocsVersion = std::move(SurrealDocsVersion);
}

const std::vector<std::string> &
    udocs_processor::DynamicPageData::GetAvailableVersions() const {
  return AvailableVersions;
}

void udocs_processor::DynamicPageData::SetAvailableVersions(
    std::vector<std::string> AvailableVersions) {
  this->AvailableVersions = std::move(AvailableVersions);
}

int64_t udocs_processor::DynamicPageData::GetLastUpdated() const {
  return LastUpdated;
}

void udocs_processor::DynamicPageData::SetLastUpdated(int64_t LastUpdated) {
  this->LastUpdated = LastUpdated;
}

const std::string &udocs_processor::DynamicPageData::GetCanonicalPage() const {
  return CanonicalPage;
}

void udocs_processor::DynamicPageData::SetCanonicalPage(
    std::string CanonicalPage) {
  this->CanonicalPage = std::move(CanonicalPage);
}

const udocs_processor::DynamicPageData::LinkData &
    udocs_processor::DynamicPageData::GetPreviousPage() const {
  return PreviousPage;
}

void udocs_processor::DynamicPageData::SetPreviousPage(LinkData PreviousPage) {
  this->PreviousPage = std::move(PreviousPage);
}

const udocs_processor::DynamicPageData::LinkData &
    udocs_processor::DynamicPageData::GetNextPage() const {
  return NextPage;
}

void udocs_processor::DynamicPageData::SetNextPage(LinkData NextPage) {
  this->NextPage = std::move(NextPage);
}

const std::string &udocs_processor::DynamicPageData::GetInstance() const {
  return Instance;
}

void udocs_processor::DynamicPageData::SetInstance(std::string Instance) {
  this->Instance = std::move(Instance);
}

const std::string &udocs_processor::DynamicPageData::GetProject() const {
  return Project;
}

void udocs_processor::DynamicPageData::SetProject(std::string Project) {
  this->Project = std::move(Project);
}

udocs_processor::DynamicPageData::LinkData::LinkData(
    std::string Title, std::string Link)
    : Title(std::move(Title)), Link(std::move(Link)) {}

const std::string &
    udocs_processor::DynamicPageData::LinkData::GetTitle() const {
  return Title;
}

const std::string &
    udocs_processor::DynamicPageData::LinkData::GetLink() const {
  return Link;
}
