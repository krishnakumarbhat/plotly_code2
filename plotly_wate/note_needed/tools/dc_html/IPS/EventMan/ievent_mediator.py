from abc import ABC, abstractmethod

class IEventMediator(ABC):
    @abstractmethod
    def notify_event(self, sender, event): pass
