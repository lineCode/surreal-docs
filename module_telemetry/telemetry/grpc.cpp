/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/telemetry/grpc.h"

std::shared_ptr<grpc::Channel> udocs_processor::CreateChannel(
    const std::string& Address, std::optional<std::string> RootCert) {
  grpc::SslCredentialsOptions Options;
  std::shared_ptr<grpc::ChannelCredentials> ChannelCreds;
  if (RootCert) {
    Options.pem_root_certs = *RootCert;
    ChannelCreds = grpc::SslCredentials(Options);
  }

  return grpc::CreateChannel(Address, RootCert
      ? ChannelCreds : grpc::InsecureChannelCredentials());
}
