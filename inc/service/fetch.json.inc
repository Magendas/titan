<?php

class FetchJSON{

	// @ usage : FetchJSON::get()
	public static function get($file_path="") {
		$is_not_file_exists = !file_exists($file_path);
		if($is_not_file_exists) {
			// TEST
			echo "FetchJSON.get / \$is_not_file_exists : $is_not_file_exists<br/>";
			return;
		}

		// 싱글턴 객체가 없습니다. 새로 만듭니다.
		$service_const_json_str = file_get_contents($file_path);
		$service_const_json = json_decode($service_const_json_str);

		return $service_const_json;
	}

}

?>
