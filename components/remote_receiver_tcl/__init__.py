import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import binary_sensor, remote_receiver, remote_base
from esphome.components.remote_base import register_dumper, declare_protocol, register_binary_sensor, register_trigger, register_action
from esphome.const import (
    CONF_COMMAND_REPEATS,
    CONF_DATA,
    CONF_TRIGGER_ID,
    CONF_NBITS,
    CONF_ADDRESS,
    CONF_COMMAND,
    CONF_CODE,
    CONF_PULSE_LENGTH,
    CONF_SYNC,
    CONF_ZERO,
    CONF_ONE,
    CONF_INVERTED,
    CONF_PROTOCOL,
    CONF_GROUP,
    CONF_DEVICE,
    CONF_SECOND,
    CONF_STATE,
    CONF_CHANNEL,
    CONF_FAMILY,
    CONF_REPEAT,
    CONF_WAIT_TIME,
    CONF_TIMES,
    CONF_TYPE_ID,
    CONF_CARRIER_FREQUENCY,
    CONF_RC_CODE_1,
    CONF_RC_CODE_2,
    CONF_MAGNITUDE,
    CONF_WAND_ID,
    CONF_LEVEL,
    CONF_DELTA,
    CONF_ID,
    CONF_BUTTON,
    CONF_CHECK,
)
from esphome.core import coroutine
from esphome.schema_extractors import SCHEMA_EXTRACT, schema_extractor
from esphome.util import Registry, SimpleRegistry

AUTO_LOAD = ["binary_sensor"]

CONF_RECEIVER_ID = "receiver_id"
CONF_TRANSMITTER_ID = "transmitter_id"
CONF_FIRST = "first"

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA

ns = remote_base_ns = cg.esphome_ns.namespace("remote_base")
RemoteProtocol = ns.class_("RemoteProtocol")
RemoteReceiverListener = ns.class_("RemoteReceiverListener")
RemoteReceiverBinarySensorBase = ns.class_(
    "RemoteReceiverBinarySensorBase", binary_sensor.BinarySensor, cg.Component
)
RemoteReceiverTrigger = ns.class_(
    "RemoteReceiverTrigger", automation.Trigger, RemoteReceiverListener
)
RemoteTransmitterDumper = ns.class_("RemoteTransmitterDumper")
RemoteTransmittable = ns.class_("RemoteTransmittable")
RemoteTransmitterActionBase = ns.class_(
    "RemoteTransmitterActionBase", RemoteTransmittable, automation.Action
)
RemoteReceiverBase = ns.class_("RemoteReceiverBase")
RemoteTransmitterBase = ns.class_("RemoteTransmitterBase")


def templatize(value):
    if isinstance(value, cv.Schema):
        value = value.schema
    ret = {}
    for key, val in value.items():
        ret[key] = cv.templatable(val)
    return cv.Schema(ret)


REMOTE_LISTENER_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_RECEIVER_ID): cv.use_id(RemoteReceiverBase),
    }
)


REMOTE_TRANSMITTABLE_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_TRANSMITTER_ID): cv.use_id(RemoteTransmitterBase),
    }
)



# TCL
TCLData, TCLBinarySensor, TCLTrigger, TCLAction, TCLDumper = declare_protocol("TCL")
TCL_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_DATA): cv.hex_uint64_t,
        cv.Optional(CONF_COMMAND_REPEATS, default=1): cv.uint16_t,
    }
)


@register_binary_sensor("tcl", TCLBinarySensor, TCL_SCHEMA)
def tcl_binary_sensor(var, config):
    cg.add(
        var.set_data(
            cg.StructInitializer(
                TCLData,
                ("data", config[CONF_DATA]),
                ("command_repeats", config[CONF_COMMAND_REPEATS]),
            )
        )
    )


@register_trigger("tcl", TCLTrigger, TCLData)
def tcl_trigger(var, config):
    pass


@register_dumper("tcl", TCLDumper)
def tcl_dumper(var, config):
    pass


@register_action("tcl", TCLAction, TCL_SCHEMA)
async def tcl_action(var, config, args):
    template_ = await cg.templatable(config[CONF_DATA], args, cg.uint64)
    cg.add(var.set_command(template_))
    template_ = await cg.templatable(config[CONF_COMMAND_REPEATS], args, cg.uint16)
    cg.add(var.set_command_repeats(template_))
