SUMMARY = "Vehicle Controller Embedded C++ Application"
DESCRIPTION = "Event-driven embedded controller simulator with GPIO abstraction, UART parser, event queue, RTOS-style tasks, and state machine."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://vehicle-controller.service"

inherit systemd cmake

SYSTEMD_SERVICE:${PN} = "vehicle-controller.service"
SYSTEMD_AUTO_ENABLE:${PN} = "enable"

do_install:append() {
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/vehicle-controller.service ${D}${systemd_system_unitdir}/vehicle-controller.service
}

FILES:${PN} += "${systemd_system_unitdir}/vehicle-controller.service"