<?php

// common setting
include_once("../common.inc");

// expire cache
CookieManager::expireCookie($param->COOKIELOGIN);

TitanLinkManager::go(TitanLinkManager::$LOG_IN);

?>