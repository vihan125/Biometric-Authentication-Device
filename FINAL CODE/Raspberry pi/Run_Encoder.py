from abc import ABC, abstractmethod
import os

class Abstract_encoder(ABC):

    @abstractmethod
    def run_encoder(self):
        pass

class WSQ_Encoder(Abstract_encoder):
    
    def run_encoder(self):
        os.system('make')
        os.system('./encode')
        os.system('make clear')
        return 1
