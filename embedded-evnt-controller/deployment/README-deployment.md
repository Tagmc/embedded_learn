# Vehicle Controller Deployment

This project can be built as a Linux user-space application and deployed as a systemd service.

## Build on Linux

```bash
cmake -S . -B build
cmake --build build
./build/embedded_event_controller