<?php
import("com.laurus.core.websocket.actions.IWebSocketAction");

/**
 * Description of CoreActionWebSocket
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */

abstract class CoreActionWebSocket implements IWebSocketAction{
    public function executeInvoke() {
        return $this->invoke(new WebsocketRequest(),new WebscoketResponse());
    }
    protected function validateRequestParameters(WebsocketRequest $wr ,$mandatoryparams){
        foreach ($mandatoryparams as $key => $value) {
            if(!TUtilString::isValidString($wr->getParameter($key)))
                throw new ValidationException($value ."  is mandatory");
        }
    }
    
}

?>
