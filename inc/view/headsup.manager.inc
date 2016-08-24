<?php

class HeadsUpManager{

	public static $HTML_TAG="HTML_TAG";
	public static $TITLE="TITLE";
	public static $ID="ID";

	// @ usage : HeadsUpManager::$COLOR_BLUE
	public static $COLOR_BLUE="COLOR_BLUE";
	// @ usage : HeadsUpManager::$COLOR_GREEN
	public static $COLOR_GREEN="COLOR_GREEN";
	// @ usage : HeadsUpManager::$COLOR_YELLOW
	public static $COLOR_YELLOW="COLOR_YELLOW";
	// @ usage : HeadsUpManager::$COLOR_RED
	public static $COLOR_RED="COLOR_RED";

	// @ Usage : HeadsUpManager::get($img_url="", $img_width=200, $on_error_img_url="", $meta_data_obj=null);
	// @ Desc : 화면에 경고 메시지 노출. Alert Window를 대신함.
	public static function get($id="", $message="", $color="", $meta_data_obj=null) {

		if(empty($message)) {
			return;
		}

		$headsup_std = new stdClass();
		
		$meta_data_json_str = "";
		if(!is_null($meta_data_obj)) {
			$meta_data_json_str = json_encode($meta_data_obj);
		}

		$class_color = "alert-success";
		if(!empty($color)) {
			if(strcmp(HeadsUpManager::$COLOR_BLUE, $color) == 0){
				$class_color = "alert-info";
			} else if(strcmp(HeadsUpManager::$COLOR_GREEN, $color) == 0){
				$class_color = "alert-success";
			} else if(strcmp(HeadsUpManager::$COLOR_YELLOW, $color) == 0){
				$class_color = "alert-warning";
			} else if(strcmp(HeadsUpManager::$COLOR_RED, $color) == 0){
				$class_color = "alert-danger";
			}
		}

		$headsup_tag = "" 
		. "<div id=\"$id\" class=\"alert $class_color\" role=\"alert\"><strong><span id=\"title\">$message</span></strong><span id=\"msg\" style=\"margin-left:5px;\"></span></div>"
		;

		$headsup_std->{HeadsUpManager::$HTML_TAG} = $headsup_tag;
		$headsup_std->{HeadsUpManager::$ID} = $id;

		return $headsup_std;

	}	

}

?>
