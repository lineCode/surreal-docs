ARG BUILDBASE
FROM ${BUILDBASE}

RUN mkdir -p /build_workspace
RUN pip install pyunpack patool
SHELL ["/bin/bash", "--login", "-i", "-c"]
RUN source ~/.bashrc
RUN npm --version && npm install -g gulp
