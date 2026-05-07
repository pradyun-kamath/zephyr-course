#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

/* Resolve the external heartbeat LED via its devicetree alias. */
static const struct gpio_dt_spec led =
    GPIO_DT_SPEC_GET(DT_ALIAS(app_led), gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    int ret;

    if (!gpio_is_ready_dt(&led)) {
        LOG_ERR("Heartbeat LED GPIO controller is not ready");
        return 0;
    }

    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        LOG_ERR("Failed to configure heartbeat LED pin: %d", ret);
        return 0;
    }

    LOG_INF("Heartbeat LED on port %s pin %d — period %d ms",
        led.port->name, led.pin, CONFIG_LED_BLINK_SLEEP_TIME_MS);

    while (1) {
        ret = gpio_pin_toggle_dt(&led);
        if (ret < 0) {
            LOG_ERR("Failed to toggle heartbeat LED pin: %d", ret);
            return 0;
        }

        k_msleep(CONFIG_LED_BLINK_SLEEP_TIME_MS);
    }

    return 0;
}
