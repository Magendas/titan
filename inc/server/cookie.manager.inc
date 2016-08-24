<?php

class CookieManager{

	// TEST - PUBLIC
	private static $COOKIE_EXPIRE_IN_A_MONTH = 2592000;	// 3600 * 24 * 30
	private static $COOKIE_EXPIRE_IN_A_WEEK = 604800;	// 3600 * 24 * 7
	private static $COOKIE_EXPIRE_IN_A_HOUR = 3600;		// 3600

	public static function getLogInCookie() {
		return self::getCookie(self::$COOKIE_FB_USER_ID);
	}
	public static function setLogInCookie($fb_user_id=null) {

		if(empty($fb_user_id)) return;

		$tossed_cookie_value = $fb_user_id;

		self::expireLogInCookie();
		self::setCookie(self::$COOKIE_FB_USER_ID,$tossed_cookie_value,self::$COOKIE_EXPIRE_IN_A_HOUR);
	}
	public static function expireLogInCookie() {
		self::expireCookie(self::$COOKIE_FB_USER_ID);
	}

	// COOKIE - Default
	private static function setCookie($cookie_name, $cookie_value, $cookie_live_span){
		setcookie($cookie_name,$cookie_value,time()+$cookie_live_span);
	}
	public static function getCookie($cookie_name){
		$cookie_value = "";
		if(isset($_COOKIE[$cookie_name])){
			$cookie_value = $_COOKIE[$cookie_name];
		}

		return $cookie_value;
	}
	// CookieManager::expireCookie();
	public static function expireCookie($cookie_name){
		if(isset($_COOKIE[$cookie_name])){
			setcookie($cookie_name,"",time()-self::$COOKIE_EXPIRE_IN_A_HOUR);	
		}
	}

}

?>
