<?php

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of WebsocketRequest
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
class WebsocketRequest {
    private $message;
    private $messageType;
    private $params;
    private $command;
    private $paramsArr;
    private $reqConn;
    public function getReqConn() {
        return $this->reqConn;
    }

    public function setReqConn($reqConn) {
        $this->reqConn = $reqConn;
    }
    
    public function __construct() {
        $this->params=array();
       
    }


    public function getParamsArr() {
        return $this->paramsArr;
    }

    public function setParamsArr($paramsArr) {
        for($i=0;$i<$paramsArr->size();$i++){
            $paramsArrObj=$paramsArr->get($i);
            foreach ($paramsArrObj as $key => $value) {
                $this->setParameter($key, $value);
            }
            
        }
        $this->paramsArr = $paramsArr;
    }

    public function getCommand() {
        return $this->command;
    }

    public function setCommand($command) {
        $this->command = $command;
    }

    public /* String or String[]*/ function getParameter($str){
        if($str=="")return null;
        return isset($this->params[$str])?$this->params[$str]:null ;
    }
    
    public function setParameter($key,$val){
        if($key=="" || $val=="")return null;
        $this->params[$key]=$val;
    }
    
    public function getMessage() {
        return $this->message;
    }

    public function setMessage($message) {
        $this->message = $message;
    }

    public function getMessageType() {
        return $this->messageType;
    }

    public function setMessageType($messageType) {
        $this->messageType = $messageType;
    }

    public function toJson() {
         $map=new Map();
         $map->put("message",  $this->getMessage());
         $map->put("messageType",  $this->getMessageType());
         $map->put("paramsArr",  $this->getParamsArr());
         
         
         return $map->toJson();
     }
}

?>
