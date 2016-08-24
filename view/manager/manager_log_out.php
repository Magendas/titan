<?php

// common setting
include_once("../../common.inc");

// expire cache
CookieManager::expireCookie($param->COOKIELOGIN);
CookieManager::expireCookie($param->COOKIE_LOGIN_FACEBOOK);
CookieManager::expireCookie($param->COOKIE_LOGIN_GOOGLE);

TitanLinkManager::go(
	// target link
	TitanLinkManager::$ADMIN_LOG_IN
	// param_arr
	,array(
		"IS_LOG_OUT"=>$param->YES
	)
);

?>