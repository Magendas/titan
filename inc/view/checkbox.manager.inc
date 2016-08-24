<?php

class CheckBoxManager{

	public static $HTML_TAG="HTML_TAG";
	public static $ID="ID";
	// @ usage : CheckBoxManager::$ID_CHECKBOX_CHAIN_GROUP
	private static $ID_CHECKBOX_CHAIN_GROUP="CHECKBOX_CHAIN_GROUP";
	// @ usage : CheckBoxManager::$UNIQUE_GROUP_ID
	private static $UNIQUE_GROUP_ID="";
	// @ usage : CheckBoxManager::$TYPE_CHECK_ALL
	public static $TYPE_CHECK_ALL="CHECK_ALL";
	// @ usage : CheckBoxManager::$TYPE_CHECK_EACH
	public static $TYPE_CHECK_EACH="CHECK_EACH";

	// CheckBoxManager::get_checkbox_chain($meta_data_obj=null);
	// @ desc : 전체 선택 / 일부 선택을 도와주기 위한 체크박스. 그룹 id로 한번에 선택이 가능하다.
	public static function get_checkbox_chain($check_type="", $group_id="", $checkbox_id="", $value="", $meta_data_obj=null) {

		if(empty($check_type)) {
			return;
		}
		if(empty($group_id)) {
			return;
		}

		$meta_data_json_str = "";
		if(!is_null($meta_data_obj)) {
			$meta_data_json_str = json_encode($meta_data_obj);
		}

		$check_box_type = CheckBoxManager::$TYPE_CHECK_EACH;
		if(strcmp(CheckBoxManager::$TYPE_CHECK_ALL,$check_type) == 0) {
			$check_box_type = CheckBoxManager::$TYPE_CHECK_ALL;
		}

		$checkbox_tag = ""
		. "<div id=\"$group_id\" class=\"checkbox\" style=\"float:left;margin-top:8px;margin-right:3px;\">"
			. "<label>"
				. "<input id=\"$checkbox_id\" type=\"checkbox\" check_type=\"$check_box_type\" checkbox_value=\"$value\" data-toggle=\"tooltip\">"
		    . "</label>"
			// Meta data json str pool
			. "<textarea id=\"meta_data\" style=\"display:none;\">$meta_data_json_str</textarea>"
		. "</div>"
		;

		$checkbox_std = new stdClass();
		$checkbox_std->{CheckBoxManager::$ID} = $group_id;
		$checkbox_std->{CheckBoxManager::$HTML_TAG} = $checkbox_tag;		

		return $checkbox_std;
	}


}

?>
