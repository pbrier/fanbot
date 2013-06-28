'''
Created on Nov 28, 2011

@author: anton
'''
import yaml
import os
import array


class FanbotConfig():
    config             = {}
    width  = 52
    height = 18
    baudrate = 115200
    
    
    @classmethod
    def load(cls):
        if os.path.exists("config.yaml"):
            configfile = open("config.yaml", 'r')
            cls.config = yaml.load(configfile)
        if None ==  cls.config:
            cls.config = {}
        if None ==  cls.config.get('communication'):
            cls.config['communication'] = {}
        if None ==  cls.config.get('hubs'):
            cls.config['hubs'] = {}


        

    @classmethod
    def setSerialport(cls,serialport):
        comm = cls.config.get('communication')
        comm['serialport'] = serialport

    @classmethod
    def getSerialport(cls):
        comm = cls.config.get('communication')
        result = comm.get('serialport')
        if result == None:
            result = 'COM1'
            cls.setSerialport(result)
        return result


    @classmethod
    def getSimulation(cls):
        comm = cls.config.get('communication')
        result = comm.get('simulation')
        if result == None:
            result = True
            cls.setSimulation(result)
        return result
    
    @classmethod
    def setSimulation(cls,sim):
        comm = cls.config.get('communication')
        comm['simulation'] = sim

    @classmethod
    def getSimulationIP(cls):
        comm = cls.config.get('communication')
        result = comm.get('simulationIP')
        if result == None:    
            result = '127.0.0.1'
            cls.setSimulationIP(result)
        return result

    @classmethod
    def getSimulationIPPort(cls):
        comm = cls.config.get('communication')
        result = comm.get('simulationIPPort')
        if result == None:
            result = 1234
            cls.setSimulationIPPort(result)
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

        result = comm.get('proxyPort')
        if result == None:   
            result = '1234'
            cls.setProxyPort(result)
        return result


    @classmethod
    def setProxyPort(cls,port):
        comm = cls.config.get('communication')
        comm['proxyPort'] = port

    @classmethod
    def getHubIDs(cls):
        """ Return array list of id's found in config"""
        hubs = cls.config.get('hubs')
        return hubs.keys()


    @classmethod
    def getHubConfig(cls,id):
        """ Return array with with 24 integers signed, 16 bit. 
        Id is a string with hexadecimal representation of id of a hub
        if id is not found in dictionary it will be created with a config 0f 24 x -1"""
        hubs = cls.config.get('hubs')
        
        result = hubs.get(id)
        if result == None:
            result = [] 
            for i in range(0,23):
                result.append(-1)
            hubs[id] = result    

        return result

    @classmethod
    def createHub(cls,id):
        """Create and return a hub which can be added to the yaml file"""
        result = { }
        config = []
        fanbots = []
        for i in range(0,23):
            config.append(-1)
            fanbots.append("-0-")
        result['config'] = config
        result['fanbots'] = fanbots
        return result    

    @classmethod
    def getHubConfig(cls,id):
        """ Return array with with 24 integers signed, 16 bit. 
        Id is a string with hexadecimal representation of id of a hub
        if id is not found in dictionary it will be created with a config 0f 24 x -1"""
        hubs = cls.config.get('hubs')
        hub = hubs.get(id)
        if hub == None:
            hub = FanbotConfig.createHub(id)
            hubs[id] = hub
        if hub['config'] == None:
            hub['config'] = []    
        return hub['config']

    @classmethod
    def getHubFanbots(cls,id):
        """ Return array with with 24 integers signed, 16 bit. 
        Id is a string with hexadecimal representation of id of a hub
        if id is not found in dictionary it will be created with a config 0f 24 x -1"""
        hubs = cls.config.get('hubs')
        hub = hubs.get(id)
        if hub == None:
            hub = FanbotConfig.createHub(id)
            hubs[id] = hub
        if hub['fanbots'] == None:
            hub['fanbots'] = []    
        return hub['fanbots']


    
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
        print 'simulation Port  ',cls.getSimulationIPPort()   
        print 'proxy Port       ',cls.getProxyPort()   

        print '-----------------------------------------'
        print 'Dumping  communication dict -------------'
        comm = cls.config.get('communication')
        for k,v in comm.iteritems():
            print "key:",k, " value: " , v
                                            
        print '-----------------------------------------'
        print 'Dumping  hubs keys ------------------------'
        print cls.getHubIDs()
        print 'Dumping  hubss ------------------------'
        hubs = cls.config.get('hubs')
        for k in hubs.keys():
            config = FanbotConfig.getHubConfig(k)
            print "key:",k, " value: " , config
                                            
        print '-----------------------------------------'


        
if __name__ == '__main__':
    print 'Dumping yaml config document:'
    FanbotConfig.load()
    FanbotConfig.dump()
    FanbotConfig.save()
    