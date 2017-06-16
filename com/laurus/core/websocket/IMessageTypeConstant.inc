<?php

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of IMessageTypeConstant
 *
 * @author Kunal Mestri <kunal@laurus-it.com>
 */
interface IMessageTypeConstant {
    const CONNECTION_OPEN=0;
    const CONNECTION_CLOSE=1;
    const NEED_AUTHETICATION=2;
    const DO_AUTHETICATION=3;
    const UNAUTHETICATE_USER=4;
    const AUTHETICATE_USER=5; 
    const COMMAND_EXECUTE=6;
    const AUTHETICATION_FAILED=7;
    const SUCCESS=8;
    const AUTHETICATION_SUCCESS=9;
    const CLIENT_CREATION=10;
    const ERROR=11;
    const CLIENT_CREATION_FAILED=12;
    const CLIENT_CREATION_SUCCESS=13;
    
}

?>
