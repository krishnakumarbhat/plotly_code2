"""
File Name: data_event_mediator.py
Author: Bharanidharan Subramani
Email : Bharanidharan.s@aptiv.com
Description:
This module holds object/address of subscribers and
on the account of event occurrence notify event is dispatched
to all subscribers
"""

from IPS.EventMan.ievent_mediator import IEventMediator


class DataEventMediator(IEventMediator):
    def __init__(self):
        self._event_components = []

    def register(self, component):
        # print(f"DataEventMediator # register ")
        self._event_components.append(component)

    def notify_event(self, sender, event):
        # print(f"DataEventMediator # notify_event event= {event}")
        for component in self._event_components:
            if component != sender:
                component.consume_event(sender, event)
