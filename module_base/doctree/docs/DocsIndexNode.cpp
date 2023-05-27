/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/docs/DocsIndexNode.h"

#include <utility>
#include "udocs-processor/doctree/DocNodeVisitor.h"

const std::string &udocs_processor::DocsIndexNode::GetLogoPath() const {
  return LogoPath;
}

void udocs_processor::DocsIndexNode::SetLogoPath(std::string LogoPath) {
  this->LogoPath = std::move(LogoPath);
}

const std::string &udocs_processor::DocsIndexNode::GetTag() const {
  return Tag;
}

void udocs_processor::DocsIndexNode::SetTag(std::string Tag) {
  this->Tag = std::move(Tag);
}

const std::string &udocs_processor::DocsIndexNode::GetVersion() const {
  return Version;
}

void udocs_processor::DocsIndexNode::SetVersion(std::string Version) {
  this->Version = std::move(Version);
}

const std::string &udocs_processor::DocsIndexNode::GetWebSite() const {
  return WebSite;
}

void udocs_processor::DocsIndexNode::SetWebSite(std::string WebSite) {
  this->WebSite = std::move(WebSite);
}

const std::string &udocs_processor::DocsIndexNode::GetOrganization() const {
  return Organization;
}

void udocs_processor::DocsIndexNode::SetOrganization(std::string Organization) {
  this->Organization = std::move(Organization);
}

const std::string &udocs_processor::DocsIndexNode::GetCopyright() const {
  return Copyright;
}

void udocs_processor::DocsIndexNode::SetCopyright(std::string Copyright) {
  this->Copyright = std::move(Copyright);
}

const std::string &udocs_processor::DocsIndexNode::GetDocsTitle() const {
  return DocsTitle;
}

void udocs_processor::DocsIndexNode::SetDocsTitle(std::string DocsTitle) {
  this->DocsTitle = std::move(DocsTitle);
}

const std::string &udocs_processor::DocsIndexNode::GetLanguage() const {
  return Language;
}

void udocs_processor::DocsIndexNode::SetLanguage(std::string Language) {
  this->Language = std::move(Language);
}

const std::string &udocs_processor::DocsIndexNode::GetMasterPage() const {
  return MasterPage;
}

void udocs_processor::DocsIndexNode::SetMasterPage(std::string MasterPage) {
  this->MasterPage = std::move(MasterPage);
}

udocs_processor::DocsIndexNode::DocsIndexNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : DocNode(Parent, Id) {}

int udocs_processor::DocsIndexNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

const std::string &udocs_processor::DocsIndexNode::GetCopyrightUrl() const {
  return CopyrightUrl;
}

void udocs_processor::DocsIndexNode::SetCopyrightUrl(
    std::string CopyrightUrl) {
  this->CopyrightUrl = std::move(CopyrightUrl);
}

const std::string &udocs_processor::DocsIndexNode::GetImagesRoot() const {
  return ImagesRoot;
}

void udocs_processor::DocsIndexNode::SetImagesRoot(std::string ImagesRoot) {
  this->ImagesRoot = std::move(ImagesRoot);
}
