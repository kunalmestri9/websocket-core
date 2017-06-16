<?php

import("com.laurus.core.websocket.modelconfig.WebsocketModelConfig");
import("com.laurus.core.exception.ValidationException");
import("com.laurus.core.exception.MergeMoreActionsException");
import("com.laurus.core.json.JsonObjectWriter");
import("com.laurus.core.json.JSonFileUtil");
import("com.laurus.core.websocket.json.*");

class WebsocketController {

    private static $controller;
    private $wrapperRequest;
    private $actionLocator;
    protected $logger;

    const EVENT_FILE = "controller-events.json";

    private $isEventFileLoaded;

    public static function getInstance() {
        if (!self::$controller) {
            self::$controller = new WebsocketController();
        }
        return self::$controller;
    }

    public function setCommandConfig(WebsocketModelConfig $modelConfig) {
        $this->actionLocator = $modelConfig;
        $this->actionLocator->getActions();
        if ($this->actionLocator == null) {
            $this->logger->fatal("Model Config could not be loaded");
            throw new Exception("Model Config could not be loaded");
        }
    }

    //giving sigleton behavior
    private function __construct() {
        $this->logger = Logger::getLogger(__CLASS__);
        $this->isEventFileLoaded = false;
        try {
            JSonFileUtil::getJSONFromFile(self::EVENT_FILE);
            $this->isEventFileLoaded = true;
        } catch (FileException $ex) {
            $this->logger->warn("No Event file present for controller");
        }
    }

    public function execute($msg) {
        $command=$msg->getCommand();
        $webSocketRes = new WebscoketResponse();
        try {
            if ($command) {
                $commandName = $command;
                if ($this->actionLocator == null) {
                    throw new Exception("Model Config was null.WebsocketController Must know the config class");
                }
                $actions = $this->actionLocator->isCommandAvailable($commandName);
                $res = $this->executeActions($actions,$msg);
                return $res;
            }
        } catch (Exception $e) {
            $this->logger->debug("Exception");
            $this->logger->debug($e->getTraceAsString());
            $webSocketRes->setErrorContainer($e->getMessage());
            $webSocketRes->setErrorCode($e->getCode());
            return $webSocketRes->renderJson();
        }
    }

    private function executeActions($actions,$msg) {
       
        if ($actions != false) {
            foreach ($actions as $key => $value) {

                $webSocketReq=$msg;
                $webSocketRes = new WebscoketResponse();
                import($value);
                $actionName = substr(strrchr($value, "."), 1);
                $this->logger->info("Action Called : " . $actionName);
                $actionObject = new $actionName;
                try {
                    $result = $actionObject->invoke($webSocketReq, $webSocketRes);
                    $webSocketRes->setCommand($webSocketReq->getCommand());
                } catch (ValidationException $e) {
                    $this->logger->debug("Validation Exception");
                    $this->logger->debug($e->getTraceAsString());
                    $webSocketRes->setErrorContainer($e->getMessage());
                    $webSocketRes->setErrorCode($e->getCode());
                    return $webSocketRes->renderJson();
                } catch (MongoConnectionException $e) {
                    $this->logger->debug("Mongo ex");
                    $this->logger->debug($e->getTraceAsString());
                    $webSocketRes->setErrorContainer($e->getMessage());
                    $webSocketRes->setErrorCode($e->getCode());
                    return $webSocketRes->renderJson();
                } catch (Exception $e) {
                    $this->logger->debug("Exception");
                    $this->logger->debug($e->getTraceAsString());
                    $webSocketRes->setErrorContainer($e->getMessage());
                    $webSocketRes->setErrorCode($e->getCode());
                    return $webSocketRes->renderJson();
                }
                $webSocketReq->setCommand(serialize($result));
              
            }
            $this->logger->debug("Websocket response Json");
            $this->logger->debug($webSocketRes->renderJson());
            return $webSocketRes->renderJson();
        }
    }

}

?>
