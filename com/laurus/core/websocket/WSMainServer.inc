<?php
import("com.laurus.core.websocket.WebSocketConfigurationReader");
import("com.laurus.core.websocket.ServerInitiator");

/**
 * Description of WSMainServer
 *
 * @author Kunal Mestri
 */
class WSMainServer {
    private $config;
    private $logger;
    public function __construct() {
        $this->logger=  Logger::getLogger(__CLASS__);
        $this->logger->debug("Starting up websocket server");
        $this->config=WebSocketConfigurationReader::getInstance();
        $serverInitiator=new ServerInitiator($this->config);
        $this->config->getMultiControllerConfig($serverInitiator);
        echo "Server started waiting for connections";
        $serverInitiator->run();
        $this->logger->debug("Websocket server shutting down");
    }
    
    
    
}

?>
