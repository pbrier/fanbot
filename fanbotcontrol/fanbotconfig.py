'''
Created on Nov 28, 2011

@author: anton
'''
import yaml
import os


class FanbotConfig():
    config             = {}
    outputs             = {}
    bitmaps             = {}
    
    @classmethod
    def load(cls):
        if not os.path.exists("config.yaml"):
            print "Create first time config file config.yaml"
            cls.config['communication'] =               \
                {'serialport':'TBD',                    \
                 'simulation':1,'simulationIP':'TBD'    \
                }
            cls.config['bitmaps'] = {}
            cls.save()    
        configfile = open("config.yaml", 'r')
        cls.config = yaml.load(configfile)

        

    @classmethod
    def setSerialport(cls,serialport):
        comm = cls.config.get('communication')
        comm['serialport'] = serialport

    @classmethod
    def getSerialport(cls):
        comm = cls.config.get('communication')
        try:
            result = comm.get('serialport')
        except:    
            result = 'COM1'
        return result


    @classmethod
    def getSimulation(cls):
        comm = cls.config.get('communication')
        try:
            result = comm.get('simulation')
        except:
            result = True
        return result
    
    @classmethod
    def setSimulation(cls,sim):
        comm = cls.config.get('communication')
        comm['simulation'] = sim

    @classmethod
    def getSimulationIP(cls):
        comm = cls.config.get('communication')
        try:
            result = comm.get('simulationIP')
        except:    
            result = '127.0.0.1'
        return result

    @classmethod
    def getSimulationIPPort(cls):
        comm = cls.config.get('communication')
        try:
            result = comm.get('simulationIPPort')
        except:    
            result = 1234
        return result

    @classmethod
    def setSimulationIP(cls,ip):
        comm = cls.config.get('communication')
        comm['simulationIP'] = ip

    @classmethod
    def setSimulationIPPort(cls,port):
        comm = cls.config.get('communication')
        comm['simulationIPPort'] = port


    @classmethod
    def getProxyPort(cls):
        comm = cls.config.get('communication')
        try:
            result = comm.get('proxyPort')
        except Exception as e:    
            print 'getProxyPort Exception: ',e
            result = '1235'
        if not result: result = '12345'     
        return result


    @classmethod
    def setProxyPort(cls,port):
        comm = cls.config.get('communication')
        comm['proxyPort'] = port

    
    @classmethod
    def save(cls):
        print 'Saving config.yaml file  ......'
        stream = open("config.yaml", 'w')
        yaml.safe_dump(cls.config, stream,default_flow_style=False)
        stream.close()

    
    @classmethod
    def dump(cls): 
        print '-------    ----------------------------------'
        print 'Dumping  communication ----------------------'
        comm = cls.config.get('communication')
        print 'simulation:      ',cls.getSimulation()
        print 'serial port:     ',cls.getSerialport()
        print 'simulation IP    ',cls.getSimulationIP()   

        print '-----------------------------------------'
        print 'Dumping  communication dict -------------'
        comm = cls.config.get('communication')
        for key in comm.keys():
            value = comm.get(key)
            print "key:",key, " value: " , value
                                            
        print '-----------------------------------------'
        print 'Dumping  bitmaps ------------------------'
        bitmaps = cls.config.get('bitmaps')
        for key in bitmaps.keys():
            value = bitmaps.get(key)
            print "key:",key, " value: " , value
                                            
        print '-----------------------------------------'
        
if __name__ == '__main__':
    print 'Dumping yaml config document:'
    FanbotConfig.load()
    FanbotConfig.dump()
    FanbotConfig.save()
    