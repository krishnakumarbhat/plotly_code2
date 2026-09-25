import time


def test_cycle(Power_Session):
    Power_Session.switch_off()
    time.sleep(10)
    Power_Session.switch_on()
