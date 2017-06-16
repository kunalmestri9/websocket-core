<?php
use Ratchet\Wamp\WampServerInterface;

/**
 *
 * @author Kunal Mestri
 */
class BackgroundMessageInterceptor implements WampServerInterface{
    
    private $logger;
    private $config;
    private static $CFG_FOLDERNAME="cfg";
    
    
    public function __construct() {
        $this->logger=  Logger::getLogger(__CLASS__);
        $this->loadAllJobs();
    }
    private function loadAllJobs(){
        $zmqJobsConfigFile=$this->getZMQJobsConfig();
        if(!file_exists($zmqJobsConfigFile)){
            $this->logger->warn($zmqJobsConfigFile ."zmq-jobs.json not found in the application");
            $this->isCofigRead=false;
        } 
        $this->config=json_decode(file_get_contents($zmqJobsConfigFile),true);
        $this->logger->info("Config read from zmq-jobs.json");
        $this->logger->debug($this->config);

    }
    
    private function getZMQJobsConfig(){
        return  $_SERVER["APPLICATION_PATH"] . DIRECTORY_SEPARATOR . self::$CFG_FOLDERNAME  . DIRECTORY_SEPARATOR . "zmq-jobs.json";
    }
    
    public function onCall(\Ratchet\ConnectionInterface $conn, $id, $topic, array $params) {
        // No Need to ues on websocket interface
    }

    public function onClose(\Ratchet\ConnectionInterface $conn) {
        
    }

    public function onError(\Ratchet\ConnectionInterface $conn, \Exception $e) {
        
    }

    public function onOpen(\Ratchet\ConnectionInterface $conn) {
        
    }
    
    
    public function onBackgroundJobInitiated($entry) {
        if($this->config){
            try{
                $entryData = json_decode($entry, true);    
                $jobName=$entryData['jobName'];
                $this->logger->debug("Background JOB for ZMQ :" .$jobName . ": is Added");
                if (array_key_exists($jobName, $this->config)) {
                    $this->logger->debug($jobName . " is found in the configuration");
                    $object=ClassLoader::loadName($this->config[$jobName])->getInstance();
                    if($object instanceof IZMQBackgroundJob){
                        $object->executeJob(isset($entryData["data"])? $entryData["data"] : array());
                    }else{
                        $this->logger->error($this->config[$jobName]." is not instance of IZMQBackgroundJob");
                    }
                    $this->logger->debug("Background JOB for ZMQ :" .$jobName . ": is Completed");
                    return;
                }
            }catch (Exception $ex){
                $this->logger->error($ex->getMessage());
            }
        }else{
            $this->logger->debug('Background Jobs recived but seems that configuration is empty');
        }
    }
    
    public function onPublish(\Ratchet\ConnectionInterface $conn, $topic, $event, array $exclude, array $eligible) {
        
    }

    public function onSubscribe(\Ratchet\ConnectionInterface $conn, $topic) {
        
    }

    public function onUnSubscribe(\Ratchet\ConnectionInterface $conn, $topic) {
        
    }    
}

?>
