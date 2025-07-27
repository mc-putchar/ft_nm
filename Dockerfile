# FROM alpine:latest
# RUN apk add --no-cache clang libc-dev build-base

FROM debian:bookworm-slim
RUN apt-get update \
  && apt-get install -y build-essential libc-dev clang
WORKDIR /app
COPY . .
CMD ["make"]
