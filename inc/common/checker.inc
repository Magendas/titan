<?php

class Checker{

	// @ Public
	// @ Desc : 사용자가 입력한 값을 바탕으로 hashkey를 반환합니다. 입력값이 없다면 millisec 서버 시간을 기준으로 만들어줍니다.
	// @ Usage : Checker::get_hash_key($user_str="");
	public static function get_hash_key($user_str=""){
		$cur_micro_time = microtime(true);
		$hash_key = MD5("$user_str $cur_micro_time");

		return $user_str . "_" . $hash_key;
	}

	// @ Usage : Checker::is_unsigned_number
	public static function is_unsigned_number($target){
		if(	is_null($target) || !is_numeric($target) || ($target < 0)){
			return false;
		}
		return true;
	}

	// @ Desc : 1부터 시작하는 모든 자연수를 의미. ex) mysql의 primary id.
	// @ Usage : Checker::is_natural_number
	public static function is_natural_number($target){
		if(	is_null($target) || !is_numeric($target) || ($target < 1)){
			return false;
		}
		return true;
	}

	public static function is_not_natural_number($target){
		return !Checker::is_natural_number($target);
	}

	// @ Usage : Checker::is_not_unsigned_number
	public static function is_not_unsigned_number($target){
		return !Checker::is_unsigned_number($target);
	}

	// @ Usage : Checker::is_not_number
	public static function is_not_number($target){
		$is_not_number = !is_numeric($target);
		return $is_not_number;
	}

	// @ Usage : Checker::is_not_valid_array
	public static function is_not_valid_array($target){
		return !Checker::is_valid_array($target);
	}

	// @ Usage : Checker::is_valid_array
	public static function is_valid_array($target){
		if( is_null($target) || count($target) < 1 ) {
			return false;	
		}
		return true;
	}

	// @ Usage : Checker::is_not_same_str($target_head_str="", $target_tail_str="")
	public static function is_not_same_str($target_head_str="", $target_tail_str=""){
		return !Checker::is_same_str($target_head_str, $target_tail_str);
	}

	// @ Usage : Checker::is_same_str($target_head_str="", $target_tail_str="")
	public static function is_same_str($target_head_str="", $target_tail_str=""){

		if(is_null($target_head_str) || is_null($target_tail_str)) {
			return false;
		}

		if(strcmp($target_head_str, $target_tail_str) == 0) {
			return true;	
		}
		return false;
	}

	// @ Usage : Checker::get_client_ip();
	public static function get_client_ip() {

		if (!empty($_SERVER['HTTP_CLIENT_IP'])) {
	    	$ip = $_SERVER['HTTP_CLIENT_IP'];
		} elseif (!empty($_SERVER['HTTP_X_FORWARDED_FOR'])) {
	    	$ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
		} else {
	    	$ip = $_SERVER['REMOTE_ADDR'];
		}

		return $ip;
	}

	private static function get_user_agent() {
		return $_SERVER['HTTP_USER_AGENT'];
	}

	// @ Usage : Checker::get_client_os();
	public static function get_client_os() {
		
		$user_agent = Checker::get_user_agent();

	    $os_platform    =   "Unknown OS Platform";
	    $os_array       =   array(
            '/windows nt 6.2/i'     =>  'Windows 8',
            '/windows nt 6.1/i'     =>  'Windows 7',
            '/windows nt 6.0/i'     =>  'Windows Vista',
            '/windows nt 5.2/i'     =>  'Windows Server 2003/XP x64',
            '/windows nt 5.1/i'     =>  'Windows XP',
            '/windows xp/i'         =>  'Windows XP',
            '/windows nt 5.0/i'     =>  'Windows 2000',
            '/windows me/i'         =>  'Windows ME',
            '/win98/i'              =>  'Windows 98',
            '/win95/i'              =>  'Windows 95',
            '/win16/i'              =>  'Windows 3.11',
            '/macintosh|mac os x/i' =>  'Mac OS X',
            '/mac_powerpc/i'        =>  'Mac OS 9',
            '/linux/i'              =>  'Linux',
            '/ubuntu/i'             =>  'Ubuntu',
            '/iphone/i'             =>  'iPhone',
            '/ipod/i'               =>  'iPod',
            '/ipad/i'               =>  'iPad',
            '/android/i'            =>  'Android',
            '/blackberry/i'         =>  'BlackBerry',
            '/webos/i'              =>  'Mobile'
		);

	    foreach ($os_array as $regex => $value) { 

	        if (preg_match($regex, $user_agent)) {
	            $os_platform    =   $value;
	        }

	    }   

		return $os_platform;		
	}

	// @ Usage : Checker::get_client_browser();
	public static function get_client_browser() {

	    $user_agent = Checker::get_user_agent();

	    $browser        =   "Unknown Browser";
	    $browser_array  =   array(
			'/msie/i'       =>  'Internet Explorer',
			'/firefox/i'    =>  'Firefox',
			'/safari/i'     =>  'Safari',
			'/chrome/i'     =>  'Chrome',
			'/opera/i'      =>  'Opera',
			'/netscape/i'   =>  'Netscape',
			'/maxthon/i'    =>  'Maxthon',
			'/konqueror/i'  =>  'Konqueror',
			'/mobile/i'     =>  'Handheld Browser'
		);

	    foreach ($browser_array as $regex => $value) { 

	        if (preg_match($regex, $user_agent)) {
	            $browser    =   $value;
	        }

	    }

	    return $browser;		
	}
	

}

?>
