<?php

class ButtonManager{

	public static $HTML_TAG="HTML_TAG";
	public static $TITLE="TITLE";
	public static $ID="ID";
	// @ usage : ButtonManager::$ID_BUTTON_CHAIN_GROUP
	public static $ID_BUTTON_CHAIN_GROUP="BUTTON_CHAIN_GROUP";

	// @ usage : ButtonManager::$COLOR_BLUE
	public static $COLOR_BLUE="COLOR_BLUE";
	// @ usage : ButtonManager::$COLOR_GREEN
	public static $COLOR_GREEN="COLOR_GREEN";
	// @ usage : ButtonManager::$COLOR_RED
	public static $COLOR_RED="COLOR_RED";
	// @ usage : ButtonManager::$COLOR_YELLOW
	public static $COLOR_YELLOW="COLOR_YELLOW";
	// @ usage : ButtonManager::$COLOR_WHITE
	public static $COLOR_WHITE="COLOR_WHITE";


	private $title;
	private $id;
	private $color;
	public function ButtonManager($title="", $color=""){

		if(empty($title)) {
			$title = "button";
		}

		$this->title = $title;	

		// set unique id
		$button_id = Checker::get_hash_key($title);
		$this->set_id($button_id);

		$this->color = $color;

	}

	private function set_id($id=""){
		if(empty($id)) {
			return;
		}

		$this->id = $id;
	}
	public function get_id(){
		return $this->id;
	}

	private function set_title($title=""){
		if(empty($title)) {
			return;
		}

		$this->title = $title;
	}
	public function get_title(){
		return $this->title;
	}


	
	// @ Usage : ButtonManager::get_combobox();
	// @ Desc : html tag, id 및 관련 정보를 돌려줍니다.
	public function get_button() {

		$target_option_arr = null;
		if(isset($this->option_arr)) {
			$target_option_arr = $this->option_arr;	
		}

		$style_width = "";
		if(!empty($width)) {
			$style_width = "width:$width;";
		}

		$button_std = new stdClass();
		$button_std->{ButtonManager::$ID} = $this->get_id();

		$button_title = $this->get_title();
		$button_id = $this->get_id();

		$button_color_class = "btn-default";
		if(strcmp($this->color, ButtonManager::$COLOR_GREEN) == 0) {
			$button_color_class = "btn-success";
		} else if(strcmp($this->color, ButtonManager::$COLOR_BLUE) == 0) {
			$button_color_class = "btn-primary";
		} else if(strcmp($this->color, ButtonManager::$COLOR_RED) == 0) {
			$button_color_class = "btn-danger";
		} else if(strcmp($this->color, ButtonManager::$COLOR_YELLOW) == 0) {
			$button_color_class = "btn-info";
		}

		$button_tag = "";
		$button_tag = "" 
		. "<div class=\"col-md-1\" style=\"padding-left:0px;padding-right:10px;margin-right: 24px;\">"
				. "<button id=\"$button_id\" type=\"button\" class=\"btn $button_color_class\" style=\"width:100px;\">"
					// . "<span class=\"glyphicon glyphicon-eye-open\" aria-hidden=\"true\" style=\"padding-right:10px;\"></span>"
					. "<strong>$button_title</strong>"
				. "</button>"
		. "</div>"
		;

		$button_std->{ButtonManager::$HTML_TAG} = $button_tag;		

		return $button_std;

	}

	// @ Usage : ButtonManager::get_button_chain($btn_key_value_arr=null, $btn_size="", $btn_color="", $selected_key="");
	// @ Desc : 유저가 클릭할때마다 NEW -> AVAILABLE -> NOT USED 둥의 상태 변경을 할 수 있는 버튼 그룹을 만들어 줍니다.

	// ButtonManager::$SIZE_BIG
	public static $SIZE_BIG="SIZE_BIG";
	// ButtonManager::$SIZE_MEDIUM
	public static $SIZE_MEDIUM="SIZE_MEDIUM";
	// ButtonManager::$SIZE_SMALL
	public static $SIZE_SMALL="SIZE_SMALL";
	// ButtonManager::$SIZE_TINY
	public static $SIZE_TINY="SIZE_TINY";

	public static function get_button_chain($btn_key_value_arr=null, $btn_size="", $selected_key="", $btn_id="", $meta_data_obj=null) {

		if(empty($btn_key_value_arr)) {
			return;
		}

		$btn_size_class = "";
		if(strcmp(ButtonManager::$SIZE_BIG ,$btn_size) == 0) {
			$btn_size_class = "btn-lg";
		} else if(strcmp(ButtonManager::$SIZE_SMALL ,$btn_size) == 0) {
			$btn_size_class = "btn-sm";
		} else if(strcmp(ButtonManager::$SIZE_TINY ,$btn_size) == 0) {
			$btn_size_class = "btn-xs";
		}


		$meta_data_json_str = "";
		if(!is_null($meta_data_obj)) {
			$meta_data_json_str = json_encode($meta_data_obj);
		}

		// $btn_key_value_arr
		$group_id = ButtonManager::$ID_BUTTON_CHAIN_GROUP;
		$button_tag = ""
		. "<div id=\"$group_id\" class=\"col-md-1\" style=\"padding-left:0px;padding-right:10px;margin-right: 24px;\">"
		;

		for($idx = 0; $idx < count($btn_key_value_arr); $idx++) {

			$btn_key_value_obj = $btn_key_value_arr[$idx];
			$btn_key = $btn_key_value_obj->key;
			$btn_value = $btn_key_value_obj->value;
			$btn_color = "";
			// if(!is_null($btn_key_value_obj->color)) {
			if(isset($btn_key_value_obj->color)) {
				$btn_color = $btn_key_value_obj->color;
			}

			if(is_null($btn_key) || empty($btn_key)) {
				return;
			}

			$button_color_class = "btn-default";
			if(strcmp($btn_color, ButtonManager::$COLOR_GREEN) == 0) {
				$button_color_class = "btn-success";
			} else if(strcmp($btn_color, ButtonManager::$COLOR_BLUE) == 0) {
				$button_color_class = "btn-primary";
			} else if(strcmp($btn_color, ButtonManager::$COLOR_RED) == 0) {
				$button_color_class = "btn-danger";
			}

			$css_display_off = "display:none;";
			if(empty($selected_key) && $idx == 0) {
				// 지정하지 않았다면 첫번재 버튼이 선택됩니다.
				$css_display_off = "";
			} else if(strcmp($selected_key, $btn_key) == 0) {
				$css_display_off = "";
			}

			$button_tag .= ""
			. "<button id=\"$btn_id\" btn_value=\"$btn_value\" type=\"button\" class=\"btn $button_color_class $btn_size_class\" style=\"$css_display_off\">"
				. "<strong>$btn_key</strong>"
			. "</button>"
			;

		}
		$button_tag .= ""
			. "<textarea id=\"meta_data\" style=\"display:none;\">$meta_data_json_str</textarea>"
		. "</div>"
		;


		$button_std = new stdClass();
		$button_std->{ButtonManager::$ID} = $group_id;
		$button_std->{ButtonManager::$HTML_TAG} = $button_tag;		

		return $button_std;
	}


}

?>
