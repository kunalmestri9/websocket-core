<?php

/**
 * Description of WebSocketConfigurationReader
 *
 * @author Kunal Mestri
 */
class WebSocketConfigurationReader {
    private static $instance;
    private static $CFG_FOLDERNAME="cfg";
    private static $WEBSOCKET_CONFIG_FILE= "websocket.json";
    private static $logger;
    private static $isConfigRead=false;
    private static $config;
    
    
    //not allowing object creation from outside
    private function __construct() {
        self::$logger=  Logger::getLogger(__CLASS__);
        $this->readJsonWebSocketConfig();
    }
    
     /**
     * 
     * @return WebSocketConfigurationReader
     */
    public static function getInstance(){
        if(self::$instance==null){
            self::$instance=new WebSocketConfigurationReader();
            self::$logger->info("WebSocketConfigurationReader Instanciated");
        }
        return self::$instance;
    }
    
    private function readJsonWebSocketConfig(){
        $webConfigFileName=$this->getWebSocketConfigPath();
        if(!file_exists($webConfigFileName)){
            self::$logger->warn($webConfigFileName ."websocket.json not found in the application");
            self::$isConfigRead=false;
        } 
        self::$config=json_decode(file_get_contents($webConfigFileName),true);
        self::$logger->info("Config read from websocket.json");
        self::$logger->debug(self::$config);
    }
    
    private function getWebSocketConfigPath(){
        return  $_SERVER["APPLICATION_PATH"] . DIRECTORY_SEPARATOR . self::$CFG_FOLDERNAME  . DIRECTORY_SEPARATOR . self::$WEBSOCKET_CONFIG_FILE;
    }
    
    
    public function getMultiControllerConfig(\ServerInitiator $serverInitiator){
        if(isset(self::$config["multicontrollerconfig"])){
            foreach (self::$config["multicontrollerconfig"] as $key => $value) {
                //ClassLoader::loadName($value["_controller"])->getInstance()
                $classpath=$value["_controller"];
                import($classpath);
                $class=substr(strrchr($classpath,"."),1);
                if($value["config"] && is_array($value["config"])){
                    $object=new $class($value["config"]);
                }else{
                    $object=new $class();
                }
                if(isset($value["zmq-config"])){
                    self::$logger->debug("got zmq -config here");
                    $serverInitiator->createZMQListener($value["zmq-config"],$object);
                }
                $serverInitiator->route($value["url"],$object,$value["allowedOrigins"]);
            }
        }
    }
    
    
    /**
     * this is the http port that socket will bind. it should be localhost for almost type of applications
     * @return string
     */
    public function getHttpHost(){
        if(isset(self::$config["server-config"]) && isset(self::$config["server-config"]["hostname"])){
            return self::$config["server-config"]["hostname"];
        }
        return "localhost";
    }
    
    public function getPort(){
        if(isset(self::$config["server-config"]) && isset(self::$config["server-config"]["port"])){
            return self::$config["server-config"]["port"];
        }
        return "8080";
    }
    
    public function getBindingAddress(){
        if(isset(self::$config["server-config"]) && isset(self::$config["server-config"]["binding-address"])){
            return self::$config["server-config"]["binding-address"];
        }
        return "127.0.0.1";
    }
}

?>
