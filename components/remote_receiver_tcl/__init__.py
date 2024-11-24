import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components.remote_base import register_dumper, declare_protocol, register_binary_sensor, register_trigger, \
    register_action
from esphome.const import (
    CONF_DATA
)

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA

TCLData, TCLBinarySensor, TCLTrigger, TCLAction, TCLDumper = declare_protocol("TCL")
TCL_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_DATA): cv.hex_uint32_t,
    }
)

@register_binary_sensor("tcl", TCLBinarySensor, TCL_SCHEMA)
def tcl_binary_sensor(var, config):
    cg.add(
        var.set_data(
            cg.StructInitializer(
                TCLData,
                ("data", config[CONF_DATA]),
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
    template_ = await cg.templatable(config[CONF_DATA], args, cg.uint32)
    cg.add(var.set_command(template_))
