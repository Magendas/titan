<?php

class InputManager{

	public static $HTML_TAG="HTML_TAG";
	public static $TITLE="TITLE";
	public static $ID="ID";
	public static $GLOBAL_ID="AIRBORNE_VIEW_INPUT";
	public static $GLOBAL_IMAGE_INPUT_ID="AIRBORNE_IMAGE_INPUT";

	// @ usage : InputManager::$COLOR_BLUE
	public static $COLOR_BLUE="COLOR_BLUE";
	// @ usage : InputManager::$COLOR_GREEN
	public static $COLOR_GREEN="COLOR_GREEN";
	// @ usage : InputManager::$COLOR_YELLOW
	public static $COLOR_YELLOW="COLOR_YELLOW";
	
	// @ Usage : InputManager::get_view_input_max_char($title="", $max_char=-1, $text="", $header_text="", $header_color="");
	// @ Desc : max char의 수만큼 입력할 수 있는 input
	public static function get_view_input_max_char($title="", $max_char=-1, $text_on_view="", $text_on_input="", $text_length=-1, $header_text="", $header_color="", $meta_data_obj=null) {

		if(empty($title)) {
			$title = "view_input";
		}
		// set unique id
		$veiw_input_id = Checker::get_hash_key($title);

		if(Checker::is_not_natural_number($max_char)) {
			return;
		}
		if(is_null($text_length)) {
			return;
		}

		$veiw_input_std = new stdClass();
		$veiw_input_std->{InputManager::$ID} = $veiw_input_id;
		$global_id = $veiw_input_std->GLOBAL_ID = InputManager::$GLOBAL_ID;

		$veiw_input_color_class = "btn-default";
		if(strcmp($header_color, InputManager::$COLOR_GREEN) == 0) {
			$veiw_input_color_class = "btn-success";
		} else if(strcmp($header_color, InputManager::$COLOR_YELLOW) == 0) {
			$veiw_input_color_class = "btn-warning";
		} else if(strcmp($header_color, InputManager::$COLOR_BLUE) == 0) {
			$veiw_input_color_class = "btn-primary";
		}

		
		$char_cnt_left = $max_char - $text_length;

		$btn_class = "btn-default";
		$btn_disabled = "";
		if($char_cnt_left < 0) {
			// over flow!
			$btn_class = "btn-danger";
			$btn_disabled = "disabled=\"disabled\"";
		}
		
		$header_btn_tag = "";
		if(!empty($header_text)) {
			$header_btn_tag = "<button type=\"button\" class=\"btn $veiw_input_color_class btn-xs\" style=\"margin-right:5px;\">$header_text</button>";
		}
		$text_strlen_btn_tag = "<button type=\"button\" id=\"str_len\" class=\"btn $btn_class btn-xs\" style=\"float: right;\">" . $text_length . "</button>";

		$meta_data_json_str = "";
		if(!is_null($meta_data_obj)) {
			$meta_data_json_str = json_encode($meta_data_obj);
		}

		$veiw_input_tag = "" 
		// TEXT ON INPUT
		. "<div id=\"$global_id\" class=\"\">"
		. "<span id=\"text_on_input\" style=\"display:none;\">"

			. "<div class=\"input-group\">"

				// Input text length
				. "<span class=\"input-group-btn\">"
					. "<button id=\"str_len\" class=\"btn $btn_class\" type=\"button\" disabled=\"disabled\">$char_cnt_left</button>"
				. "</span>"

				// Input field
				. "<input max_char=\"$max_char\" type=\"text\" class=\"form-control col-md-6\" placeholder=\"Type text\" value=\"$text_on_input\">"

				// Button Save
				. "<span class=\"input-group-btn\">"
					. "<button id=\"btn_save\" class=\"btn btn-primary\" type=\"button\" $btn_disabled>Save</button>"
				. "</span>"


			. "</div>"
		. "</span>"

		// TEXT ON VIEW
		. "<span id=\"text_on_view\">"
			. $header_btn_tag
			. "<small id=\"text\">$text_on_view</small>"
			. $text_strlen_btn_tag

			// text storage
			. "<textarea id=\"storage\" style=\"display:none;\">$text_on_input</textarea>"				

		. "</span>"

		// Meta data json str pool
		. "<textarea id=\"meta_data\" style=\"display:none;\">$meta_data_json_str</textarea>"

		. "</div>"
		;

		$veiw_input_std->{InputManager::$HTML_TAG} = $veiw_input_tag;		

		return $veiw_input_std;

	}


	// @ Usage : InputManager::get_image_input($img_url="", $img_width=200, $on_error_img_url="", $meta_data_obj=null);
	// @ Desc : image를 보여줄 수 있고, url을 변경해서 다시 보여줌.
	public static function get_image_input($img_url_on_input="", $img_url_on_view="", $img_width=200, $on_error_img_url="", $meta_data_obj=null) {

		// 빈 이미지 주소도 허용합니다.

		$veiw_input_std = new stdClass();
		$veiw_input_std->GLOBAL_ID = InputManager::$GLOBAL_IMAGE_INPUT_ID;

		$img_width_str = "";
		if(Checker::is_not_natural_number($img_width)) {
			$img_width_str = 200 . "px";
		} else {
			$img_width_str = "$img_width" . "px";
		}

		$image_desc = "";
		$image_desc_width = $img_width - 5;
		if(isset($meta_data_obj->image_desc) && !empty($meta_data_obj->image_desc)) {
			$image_desc = $meta_data_obj->image_desc;	
		}

		$float_left = false;
		$css_float_left = "";
		if(isset($meta_data_obj->float_left) && !empty($meta_data_obj->float_left)) {
			$float_left = $meta_data_obj->float_left;
		}
		if($float_left) {
			$css_float_left = "float:left;";
		}

		$margin_left=0;
		$css_margin_left = "";
		if(isset($meta_data_obj->margin_left) && !empty($meta_data_obj->margin_left)) {
			$margin_left = intval($meta_data_obj->margin_left);
		}
		if(0 < $margin_left) {
			$css_margin_left = "margin-left:$margin_left" . "px;";
		}




		$meta_data_json_str = "";
		if(!is_null($meta_data_obj)) {
			$meta_data_json_str = json_encode($meta_data_obj);
		}

		$popup_input_uwl_width = 500;
		$margin_left = $img_width - $popup_input_uwl_width;
		$margin_left_css =  $margin_left . "px";

		$group_id = InputManager::$GLOBAL_IMAGE_INPUT_ID;

		$veiw_input_tag = "" 
		// IMAGE URL ON INPUT
		. "<div id=\"$group_id\" style=\"$css_float_left $css_margin_left\">"
		. "<span id=\"image_info_group\" style=\"display:none;\">"

			. "<div id=\"input_img_desc\" class=\"input-group xo_box_shadow_deep\" style=\"width:500px;margin-bottom: 10px;position: absolute;margin-left: $margin_left_css;margin-top: -82px;\">"

				// Input field
				. "<input type=\"text\" class=\"form-control col-md-2\" placeholder=\"Type image source\" value=\"$image_desc\" org=\"$image_desc\">"

				// Button Save
				. "<span class=\"input-group-btn\">"
					. "<button id=\"btn_save_img_desc\" class=\"btn btn-primary\" type=\"button\">Save</button>"
				. "</span>"

			. "</div>"

			. "<div id=\"input_img_url\" class=\"input-group xo_box_shadow_deep\" style=\"width:500px;margin-bottom: 10px;position: absolute;margin-left: $margin_left_css;margin-top: -42px;\">"

				// Input field
				. "<input type=\"text\" class=\"form-control col-md-2\" placeholder=\"Type image url\" value=\"$img_url_on_input\" org=\"$img_url_on_input\">"

				// Button Save
				. "<span class=\"input-group-btn\">"
					. "<button id=\"btn_save_img_url\" class=\"btn btn-primary\" type=\"button\">Save</button>"
				. "</span>"

			. "</div>"

		. "</span>"


		// IMAGE ON VIEW
		. "<span id=\"image_on_view\">"
			. "<span id=\"image_desc\" class=\"xo_text_shadow\" id=\"image_desc\" style=\"line-height: 11px;position: absolute;color: #FFF;font-size: 12px;margin-top: 5px;margin-left: 5px;width:$image_desc_width;\">$image_desc</span>"
			. "<img width=\"$img_width_str\" src=\"$img_url_on_view\" class=\"img-rounded\" onerror='this.src=\"$on_error_img_url\"'>"
		. "</span>"

		// Meta data json str pool
		. "<textarea id=\"meta_data\" style=\"display:none;\">$meta_data_json_str</textarea>"

		. "</div>"
		;

		$veiw_input_std->{InputManager::$HTML_TAG} = $veiw_input_tag;		

		return $veiw_input_std;

	}	

}

?>
