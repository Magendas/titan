<?php

class FetchJSON{

	// @ usage : FetchJSON::get()
	private static $json_arr;
	public static function get($file_path="") {

		if(empty(self::$json_arr)) {
			self::$json_arr = array();
		}

		if(isset(self::$json_arr[$file_path])) {
			// 동일한 경로의 데이터에 대해서는 저장된 데이터를 돌려줌.
			return self::$json_arr[$file_path];
		}

		$is_not_file_exists = !file_exists($file_path);
		if($is_not_file_exists) {
			return;
		}

		$service_const_json_str = file_get_contents($file_path);
		$service_const_json = json_decode($service_const_json_str);

		// 로드한 데이터를 배열에 저장.
		self::$json_arr[$file_path] = $service_const_json;

		return $service_const_json;
	}

}

?>
