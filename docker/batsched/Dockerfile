FROM nixos/nix:2.24.9

ARG BATSCHED_NUR_KAPACK_REV=774b7f56c3efc93adb73ee82a62faf5dc2fce112
ARG BATSCHED_SRC_REV=17072778995100fc90214ea4910bf5b171adfd0d

RUN mkdir -p /etc/nix \
    && echo "experimental-features = nix-command flakes" >> /etc/nix/nix.conf \
    && echo "sandbox = false" >> /etc/nix/nix.conf

WORKDIR /build
COPY batsched.nix .
COPY vendor ./vendor
RUN nix-build --argstr nurKapackRev "${BATSCHED_NUR_KAPACK_REV}" \
               --argstr batschedSrcRev "${BATSCHED_SRC_REV}" \
               batsched.nix -o /opt/batsched

ENTRYPOINT ["/opt/batsched/bin/batsched"]
