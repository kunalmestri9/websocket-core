<?php
import("com.laurus.core.modelconfig.ModelConfig");
/**
 * Description of WebsocketModelConfig
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
class WebsocketModelConfig extends ModelConfig{
     private static $instance;
    public  static function getInstance() {
        if(self::$instance==null) {
            //echo "ModelConfig::Loaded";
            self::$instance=new WebsocketModelConfig();
        }
        return self::$instance;
    }
    public function isCommandAvailable($command){
        $this->loggerConfig->info(__CLASS__ ."::". __METHOD__);
        $this->loggerConfig->info("Searching for command : " . $command );
        foreach ($this->array as $key =>$value) {
            if($key==$command){
                $this->loggerConfig->info($command." command found");
                return $value;
            }
        }
        $this->loggerConfig->warn(" Command '".$command."' not found");
        throw new Exception(" Command '".$command."' not found");
        return false;
    }
}

?>
