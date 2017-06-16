<?php
import("com.laurus.core.websocket.json.*");
/**
 * Description of IWebScoketAction
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
interface IWebSocketAction {
     public function invoke(\WebsocketRequest $wr, \WebscoketResponse $ws);
}

?>
