<?php

class ComboBoxManager{

	public static $OPTION_KEY="OPTION_KEY";
	public static $OPTION_VALUE="OPTION_VALUE";
	public static $HTML_TAG="HTML_TAG";
	public static $COMBO_BOX_ID="COMBO_BOX_ID";

	private $option_arr;

	public function ComboBoxManager(){}

	public function get_option_arr() {
		return $this->option_arr;
	}

	// @ Desc : ComboBox에서 사용해야 할 option 객체를 만들어 추가해 줍니다.
	public function add_combobox_option($key="", $value="") {
		if(empty($key)) {
			return;
		}
		if(is_null($this->option_arr)) {
			$this->option_arr = array();
		}

		// $value는 빈값을 허용합니다. 모든 값은 문자열로 처리됩니다.

		$option = new stdClass();
		$option->{ComboBoxManager::$OPTION_KEY} = $key;
		$option->{ComboBoxManager::$OPTION_VALUE} = $value;

		array_push($this->option_arr, $option);
	}

	// @ Usage : ComboBoxManager::is_valid_combobox_option_arr($target_option_arr=null);
	// @ Desc : ComboBox에서 사용해야 할 option_arr를 만들어 줍니다.
	private static function is_valid_combobox_option_arr($target_option_arr=null) {

		if(empty($target_option_arr)) {
			return false;
		}

		for($idx = 0; $idx < count($target_option_arr); $idx++) {
			$target_option = $target_option_arr[$idx];

			if(is_null($target_option->{ComboBoxManager::$OPTION_KEY})) {
				return false;
			}
		}

		return true;
	}
	
	// @ Usage : ComboBoxManager::get_combobox();
	// @ Desc : 콤보 박스 html tag 및 관련 정보를 돌려줍니다.
	public function get_combobox($selected_key="", $width="", $combobox_id="") {

		$target_option_arr = null;
		if(isset($this->option_arr)) {
			$target_option_arr = $this->option_arr;	
		}
		

		$style_width = "";
		if(!empty($width)) {
			$style_width = "width:$width;";
		}

		$combobox_std = new stdClass();
		$combobox_std->{ComboBoxManager::$COMBO_BOX_ID} = $combobox_id;

		$combobox_tag = "";
		$combobox_tag .= 
		"<select id=\"$combobox_id\" class=\"form-control\" name=\"list_search_tab\" style=\"$style_width;float:left;margin-right:10px;\">";
		
		if(isset($target_option_arr) && !empty($target_option_arr)) {
			foreach ($target_option_arr as $option) {

				$key = $option->{ComboBoxManager::$OPTION_KEY};
				$value = $option->{ComboBoxManager::$OPTION_VALUE};

				if(!empty($selected_key) && strcmp($value,$selected_key) == 0) {
					$combobox_tag .= "<option value=\"$value\" selected>$key</option>";
				} else {
					$combobox_tag .= "<option value=\"$value\">$key</option>";
				}
			    
			}
		}
		$combobox_tag .= "</select>";

		$combobox_std->{ComboBoxManager::$HTML_TAG} = $combobox_tag;		

		return $combobox_std;

	}
}

?>
