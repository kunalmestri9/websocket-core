<?php

import("com.laurus.core.websocket.controller.WebsocketController");
import("com.laurus.core.websocket.json.*");
import("com.laurus.core.websocket.IMessageTypeConstant");
import("com.laurus.core.json.JsonToObjectConverter");

/**
 * Description of CommandExecuter
 *
 * @author Amruta Vichare <amruta.vichare@laurus-it.com>
 */
class CommandExecuter implements Ratchet\MessageComponentInterface {

    protected $clients;
    protected $auth;
    private $object;
    private $controller;
    private $isAuthenticationNeeded;

    public function __construct($config = false) {
        $this->isAuthenticationNeeded = false;
        if ($config != false) {
            if ($config["isAuthEnable"] == "true") {
                $this->isAuthenticationNeeded = true;
                $this->object = ClassLoader::loadName($config["authenticator"])->getInstance();
            }
        }
        $this->clients = new \SplObjectStorage;
        $this->logger = Logger::getLogger(__CLASS__);
        $modelConfig = WebsocketModelConfig::getInstance();
        //4. Get the instance of the controller
        $this->controller = WebsocketController::getInstance();
        //5. Set the config to controller
        $this->controller->setCommandConfig($modelConfig);
        //6.Ask the controller to execute the related tasks
    }

    public function onClose(\Ratchet\ConnectionInterface $conn) {
        $this->clients->detach($conn);
        $this->logger->debug("conn colse");
        $msg=new WebsocketRequest();
        $msg->setCommand("dregid");
        $msg->setParameter("resId", $conn->resourceId);
        $res = $this->controller->execute($msg);
        $object = new Map();
        $object->put("message", "Connection {$conn->resourceId} has disconnected\n");
        $object->put("messageType", IMessageTypeConstant::CONNECTION_CLOSE);
        $conn->send($object->toJson());
    }

    public function onError(\Ratchet\ConnectionInterface $conn, \Exception $e) {
        $conn->close();
    }

    public function onMessage(\Ratchet\ConnectionInterface $from, $msgObj) {
        $this->logger->debug($msgObj);
        $converter = new JsonToObjectConverter();
        /* @var $msg WebsocketRequest */
        $msg = $converter->convertToObject($msgObj, "com.laurus.core.websocket.json.WebsocketRequest");
        $this->logger->debug($msg);
        $msg->setReqConn($from);
        if (IMessageTypeConstant::DO_AUTHETICATION == $msg->getMessageType()) {
            $this->logger->debug("Do authetication");
            $this->logger->debug($this->isAuthenticationNeeded);
            if (!$this->isAuthenticationNeeded) {
                $this->clients->attach($from);
                $data = $this->getMsgResponse("Authetication Successful", IMessageTypeConstant::AUTHETICATION_SUCCESS);
                $from->send($data->toJson());
                return;
            }
            if ($this->object instanceof IWebSocketAuthetication && $this->isAuthenticationNeeded) {
                try {
                    $this->logger->debug($this->isAuthenticationNeeded);
                    if ($this->isAuthenticationNeeded) {
                        $this->object->autheticateUser($msg, $from);
                    }
                    $this->clients->attach($from);
                    $data = $this->getMsgResponse("Authetication Successful", IMessageTypeConstant::AUTHETICATION_SUCCESS);
                    $from->send($data->toJson());
                    return;
                } catch (\Exception $ex) {
                    $data = $this->getMsgResponse("Authetication Failed", IMessageTypeConstant::AUTHETICATION_FAILED);
                    $from->send($data->toJson());
                    $this->logger->error($ex);
                }
            } else {
                $this->logger->error("MWebSocketAuthetication is not instance of IWebSocketAuthetication");
            }
        }
//        else if (IMessageTypeConstant::CLIENT_CREATION == $msg->getMessageType()) {
//            if ($this->object instanceof IWebSocketAuthetication) {
//                if ($this->clients->contains($from)) {
//                    try {
//                        $this->object->clientCreation($msg, $from);
//                        
//                        $data = $this->getMsgResponse("Client Created Successful", IMessageTypeConstant::CLIENT_CREATION_SUCCESS);
//                        $from->send($data->toJson());
//                        return;
//                    } catch (\Exception $ex) {
//                        $data = $this->getMsgResponse("Client Creation Failed", IMessageTypeConstant::CLIENT_CREATION_FAILED);
//                        $from->send($data->toJson());
//                        $this->logger->error($ex);
//                    }
//                } else {
//                    $data = $this->getMsgResponse("Authetication Failed", IMessageTypeConstant::AUTHETICATION_FAILED);
//                    $from->send($data->toJson());
//                }
//            } else {
//                $this->logger->error("MWebSocketAuthetication is not instance of IWebSocketAuthetication");
//            }
//        } 
        else if (IMessageTypeConstant::COMMAND_EXECUTE == $msg->getMessageType()) {
            if ($this->clients->contains($from)) {
                $res = $this->controller->execute($msg);
                $from->send($res);
            } else {
                $data = $this->getMsgResponse("Authetication Failed", IMessageTypeConstant::AUTHETICATION_FAILED);
                $from->send($data->toJson());
            }
        }
    }

    private function getMsgResponse($msg, $msgType) {
        $object = new WebsocketRequest();
        $object->setMessage($msg);
        $object->setMessageType($msgType);
        return $object;
    }

    public function onOpen(\Ratchet\ConnectionInterface $conn) {

        $object = new WebscoketResponse();
        $object->setSuccessMessage("New connection!({$conn->resourceId})");
        $object->setMessageType(IMessageTypeConstant::NEED_AUTHETICATION);
        $conn->send($object->renderJson());
    }

    public function onBackgroundJobInitiated($json) {
        if ($json) {
            $converter = new JsonToObjectConverter();
            /* @var $msg WebsocketRequest */
            $msg = $converter->convertToObject($json, "com.laurus.core.websocket.json.WebsocketRequest");
            $res = $this->controller->execute($msg);
        }
    }

//put your code here
}

?>
