<?php

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of WebscoketResponse
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
class WebscoketResponse {
    private $errorContainer;
    private $errorCode;
    private $successMessage;
    private $command;
    private $messageType;
    public function getCommand() {
        return $this->command;
    }

    public function setCommand($command) {
        $this->command = $command;
    }

    public function getMessageType() {
        return $this->messageType;
    }

    public function setMessageType($messageType) {
        $this->messageType = $messageType;
    }

    public function getSuccessMessage() {
        return $this->successMessage;
    }

    public function setSuccessMessage($successMessage) {
        $this->successMessage = $successMessage;
    }

    public function getErrorContainer() {
        return $this->errorContainer;
    }

    public function setErrorContainer($errorContainer) {
        $this->errorContainer = $errorContainer;
    }

    public function getErrorCode() {
        return $this->errorCode;
    }

    public function setErrorCode($errorCode) {
        $this->errorCode = $errorCode;
    }

    public function __construct() {
        
    }
    public function setJsonResponse($arr){
        $this->arr=$arr;
       
    }
    
    public function renderJson(){
        $jsonBean=$this->arr;
        if($this->getErrorContainer()!=null){
            $jsonBean=new ErrorResponse($this->getErrorContainer());
            $jsonBean->setCode($this->getErrorCode());
        }
        if($jsonBean==null){
            //Assign default success response
            $jsonBean=new SuccessResponse($this->getSuccessMessage(),null,$this->getMessageType(),$this->getCommand());
        }
       
        //var_dump($jsonBean);echo "<br/>kunal";
        if($jsonBean!=null){
            header('Content-type: application/json');
            if(!($jsonBean instanceof ErrorResponse)  && !($jsonBean instanceof SuccessResponse) ){
                $newBean=new SuccessResponse($this->getSuccessMessage(),$jsonBean,$this->getMessageType(),$this->getCommand());
                return $newBean->toJson();
            }else{
                return $jsonBean->toJson();
            }
            
        }
    }
}

?>
