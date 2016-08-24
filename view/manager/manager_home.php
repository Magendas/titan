<?php

	// @ common setting
	include_once("../../common.inc");
	
	$preprocessor = 
	new TitanPreprocessor(
		// $mysql_interface=null
		$mysql_interface
		// $permission_arr=null / null for everyone
		, array(
			$const->USER_PERMISSION_CODE_MANAGER			// Admin(모든거, Quota 설정)
		)
	);
	$PROPS = $preprocessor->get_props();

	// REDIRECT ON ERROR
	if(	$PROPS->SUCCESS == false ) {

		TitanLinkManager::go(
			// target link
			TitanLinkManager::$ADMIN_LOG_IN
			// param_arr
			,array(
				"FACEBOOK_USER_ID"=>$PROPS->FACEBOOK_USER_ID
				,"GOOGLE_USER_ID"=>$PROPS->GOOGLE_USER_ID
				,"REASON"=>$PROPS->ERROR
			)
		);
	}
	
	$user_quiz_progress = 
	$mysql_interface->select_user_quiz_progress($PROPS);
	$PROPS->user_quiz_progress = $user_quiz_progress;

	// 해당 언어에만 풀은 퀴즈 갯수. Quota 제한 확인을 위해서 필요. - 화면에 메시지 노출해야함.
	$quiz_solved_total_cnt_on_language = 
	$mysql_interface->select_quiz_solved_total_cnt_on_language($PROPS);
	$PROPS->quiz_solved_total_cnt_on_language = intval($quiz_solved_total_cnt_on_language);

	$has_enough_quota = false;
	$PROPS->user_quota = intval($PROPS->USER_INFO->__quota);
	if( (-1 < $quiz_solved_total_cnt_on_language) && 
		($quiz_solved_total_cnt_on_language < $PROPS->user_quota)) {

		$has_enough_quota = true;
	}
	$PROPS->has_enough_quota = $has_enough_quota;

	// @ required
	$PROPS->{$param->QUERY_FEEDBACK} = $mysql_interface->get_feedback();
	$mysql_interface->close();

?>



<html>
<head>
<?php
	// @ required
	ViewRenderer::render($PROPS->HEAD_FILE_PATH,$PROPS->HEAD_VIEW_RENDER_VAR_ARR);	
?>
</head>


<body role="document">

	<!-- lumino begins -->
	<?php
	$view_render_var_arr = 
	array(
		"[__USER_NAME__]"=>$PROPS->{$param->USER_NICKNAME_N_PERMISSION}
		, "[__ACTIVE_HOME__]"=>"active"
		, "[__SERVICE_ROOT__]"=>$PROPS->SERVICE_ROOT_PATH
	);
	ViewRenderer::render($PROPS->{$param->NAV_FILE_PATH},$view_render_var_arr);
	?>

	<div id="container" class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main">

		<!-- <div class="well" style="margin-top:20px;"> -->

<?php

		if($PROPS->IS_MOBILE) {
			echo "<form class=\"form-horizontal\" style=\"margin-bottom: 0px;\">";
		} else {
			echo "<form class=\"form-horizontal\" style=\"margin-top: 15px;\">";
		}
		
		
		$headsup_msg = "Welcome!";
		$headsup_color = HeadsUpManager::$COLOR_BLUE;
		if($PROPS->XOGAMES_JSON_STATUS_REPORT->SUCCESS == false && !empty($PROPS->XOGAMES_JSON_STATUS_REPORT->ERROR_MSG)) {
			$headsup_msg = $PROPS->XOGAMES_JSON_STATUS_REPORT->ERROR_MSG;
			$headsup_color = HeadsUpManager::$COLOR_YELLOW;
		}
// HEADS UP - INIT
		$headsup = HeadsUpManager::get("xogames_headsup", $headsup_msg, $headsup_color);
		$HTML_TAG = $headsup->{HeadsUpManager::$HTML_TAG};
		echo "$HTML_TAG";
// HEADS UP - DONE


// CONTROL BAR
echo "<ul class=\"list-group\" style=\"margin-bottom:10px;\">";
	echo "<li class=\"list-group-item\" style=\"padding-bottom:44px;\">";

		// COMBO BOX - REGION
		$COMBO_BOX_REGION = $PROPS->{$param->COMBO_BOX_SET}->{$param->COMBO_BOX_REGION};
		$HTML_TAG = $COMBO_BOX_REGION->{ComboBoxManager::$HTML_TAG};
		echo "$HTML_TAG";

		// COMBO BOX - LANGUAGE
		$COMBO_BOX_LANGUAGE = $PROPS->{$param->COMBO_BOX_SET}->{$param->COMBO_BOX_LANGUAGE};
		$HTML_TAG = $COMBO_BOX_LANGUAGE->{ComboBoxManager::$HTML_TAG};
		echo "$HTML_TAG";

	echo "</li>";
echo "</ul>";
// CONTROL BAR	

			echo "</form>";
?>			





				<!-- quiz progress -->
				<?php

				// $QUIZ_CATEGORY_ARR
				for ($i=0; $i < count($PROPS->QUIZ_CATEGORY_ARR); $i++) { 

					$quiz_category = $PROPS->QUIZ_CATEGORY_ARR[$i];

					$quiz_category_info = null;
					if(!is_null($user_quiz_progress)) {
						$quiz_category_info = $user_quiz_progress->{$quiz_category};
					}
					if(is_null($quiz_category_info)) {
						continue;
					}

					$quiz_cnt_arr = explode("/",$quiz_category_info);
					$user_quiz_solve_cnt = intval($quiz_cnt_arr[0]);
					$total_quiz_cnt = intval($quiz_cnt_arr[1]);

					$progress = 0;
					if(0 < $total_quiz_cnt) {
						$progress = round(($user_quiz_solve_cnt * 1000) / $total_quiz_cnt)/10;	
					}

					$category_code = 
					$param->get_category_code(
						// $region=""
						$PROPS->QUIZ_REGION
						//, $language=""
						, $PROPS->QUIZ_LANGUAGE
						//, $category=""
						, $quiz_category
					);
					// REMOVE ME
					// $category_code = $PROPS->QUIZ_CATEGORY_CODE_SET->{$quiz_category};

					$user_access_token = 
					$param->get_user_access_token(
						$PROPS->QUIZ_REGION
						, $quiz_category
					);
					if(strcmp($quiz_category, $param->QUIZ_CATEGORY_X_SANDBOX) == 0) {
						// 1. 샌드 박스는 무조건 화면에 노출해줍니다.
						$has_access = true;	
					} else {
						// 2. 그 이외의 카테고리는 접근 권한을 검사합니다.
						$has_access = false;

						foreach($PROPS->CATEGORY_ACCESS_ARR as $category_access) {
							if(strcmp($user_access_token, $category_access) == 0) {
								$has_access = true;
								break;
							}
						} // end for
					} // end if

					$quiz_category_color = $PROPS->xogames_param->QUIZ_CATEGORY_COLOR[$i];
					$css_color = "color:" . $quiz_category_color . ";";

					if($has_access) {

						echo "<div id=\"$quiz_category\" quiz_cnt=\"$total_quiz_cnt\" quiz_solved_cnt=\"$user_quiz_solve_cnt\" class=\"col-xs-6 col-md-3\" style=\"padding-left:0px;\">";
							echo "<div class=\"panel panel-default\">";
								echo "<div class=\"panel-body easypiechart-panel\">";
									echo "<h4 style=\"$css_color\"><strong>$category_code</strong></h4>";
									echo "<small style=\"$css_color\">$user_quiz_solve_cnt / $total_quiz_cnt</small>";
									echo "<div class=\"easypiechart\" id=\"easypiechart-ocean\" data-percent=\"$progress\" ><span class=\"percent\" style=\"$css_color\">$progress%</span>";
									echo "</div>";
								echo "</div>";
							echo "</div>";
						echo "</div>";

					}

				} // end foreach
				// echo "</div>";
				?>

			<!-- </form> -->

		<!-- well -->
		<!-- </div> -->

	</div>

<script>

// php to javascript sample

//PROPS
var PROPS = <?php echo json_encode($PROPS);?>;
var user_quiz_progress = PROPS.user_quiz_progress;
if(_v.is_not_valid_array(PROPS.CATEGORY_ACCESS_ARR)) {
	console.log("Warning!\nYou have no right to access any quiz.\nPlease ask the quiz manager for your permission.");
}















// on change select event
var COMBO_BOX_ID = PROPS.COMBO_BOX_SET.COMBO_BOX_REGION.COMBO_BOX_ID;
var select_region_jq = $("select#" + COMBO_BOX_ID);
select_region_jq.change(function(){
	var selected_value = $(this).val();

	_link.go_there_post(
		_link.SELF
		, _param
		.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
		.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)
		.get(PROPS.PARAM_SET.QUIZ_REGION, selected_value)
		.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, PROPS.QUIZ_LANGUAGE)
	);

});
var COMBO_BOX_ID = PROPS.COMBO_BOX_SET.COMBO_BOX_LANGUAGE.COMBO_BOX_ID;
var select_language_jq = $("select#" + COMBO_BOX_ID);
select_language_jq.change(function(){
	var selected_value = $(this).val();

	_link.go_there_post(
		_link.SELF
		, _param
		.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
		.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)
		.get(PROPS.PARAM_SET.QUIZ_REGION, PROPS.QUIZ_REGION)
		.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, selected_value)
	);

});



// EVENT HEADS-UP 
// xogames_headsup
var xogames_headsup_jq = $("div#xogames_headsup");
abc_headsup.set(xogames_headsup_jq);
if(PROPS.GD_LIBLARY == null || !PROPS.GD_LIBLARY) {
	abc_headsup._show_warning("Warning!","GD Library is not valid!");
}




// set event on category progress
var quiz_category = PROPS.xogames_param.QUIZ_CATEGORY
var has_enough_quota = PROPS.has_enough_quota;
if(!has_enough_quota) {
	abc_headsup._show_warning("Warning!","Not enough quota!");
}
for (var i = 0; i < quiz_category.length; i++) {

	var category_name = quiz_category[i];
	var category = user_quiz_progress[category_name];
    var category_jq = $("div#" + category_name);
    if(category_jq == null || category_jq.length == 0) {
    	continue;
    }

	var quiz_cnt = parseInt(category_jq.attr("quiz_cnt"));
	var quiz_solved_cnt = parseInt(category_jq.attr("quiz_solved_cnt"));

	if(0 < quiz_cnt && has_enough_quota) {

	    category_jq.click(function(e) {

			var _self_jq = $(this);	    	
			// click 하는 느낌 만들기.
			_self_jq.animate({opacity:.5},200);

	    	var _self_jq = $(this);
	    	var selected_category = _self_jq.attr("id");

	    	var region = select_region_jq.val();
	    	var language = select_language_jq.val();

	    	var link = _link.ADMIN_QUIZ_SIMULATOR;
	    	if(PROPS.IS_MOBILE) {
				link = _link.ADMIN_QUIZ_SIMULATOR_MOBILE;
	    	}

			_link.go_there_post(
				link
				, _param
				.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
				.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)
				.get(PROPS.PARAM_SET.QUIZ_REGION, region)
				.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, language)
				.get(PROPS.PARAM_SET.QUIZ_CATEGORY, selected_category)
			);

	   	});

	   	category_jq.mouseenter(function(e){

	   		var _self_jq = $(this);
	   		var panel_body_jq = _self_jq.find("div.panel");

	   		var background_color_obj = panel_body_jq.css("background-color");
	   		var background_color_hex = _color.rgb_to_hex(background_color_obj);
	   		var color_obj = panel_body_jq.css("color");
	   		var color_hex = _color.rgb_to_hex(color_obj);
	   		var color_focusing_hex = "#30a5ff";

	   		var title_color_obj = panel_body_jq.find("h4").css("color");
	   		var title_color_hex = _color.rgb_to_hex(title_color_obj);
	   		panel_body_jq.find("h4").css("color","#fff");

	   		var percent_color_obj = panel_body_jq.find("span.percent").css("color");
	   		var percent_color_hex = _color.rgb_to_hex(percent_color_obj);
	   		panel_body_jq.find("span.percent").css("color","#fff");

	   		var quiz_cnt_color_obj = panel_body_jq.find("small").css("color");
	   		var quiz_cnt_color_hex = _color.rgb_to_hex(title_color_obj);
	   		panel_body_jq.find("small").css("color","#fff");

	   		panel_body_jq.attr("background_color_hex",background_color_hex);
	   		panel_body_jq.attr("color_hex",color_hex);
	   		panel_body_jq.attr("title_color_hex",title_color_hex);
	   		panel_body_jq.attr("percent_color_hex",percent_color_hex);
	   		panel_body_jq.attr("quiz_cnt_color_hex",quiz_cnt_color_hex);

	   		panel_body_jq.css("background-color",color_focusing_hex);

	   	});

	   	category_jq.mouseleave(function(e){

	   		var _self_jq = $(this);
	   		var panel_body_jq = _self_jq.find("div.panel");

	   		var background_color_hex = panel_body_jq.attr("background_color_hex");
	   		var color_hex = panel_body_jq.attr("color_hex");
	   		var title_color_hex = panel_body_jq.attr("title_color_hex");
	   		var percent_color_hex = panel_body_jq.attr("percent_color_hex");
	   		var quiz_cnt_color_hex = panel_body_jq.attr("quiz_cnt_color_hex");

	   		panel_body_jq.css("background-color",background_color_hex);
	   		panel_body_jq.css("color",color_hex);
	   		panel_body_jq.find("h4").css("color",title_color_hex);
	   		panel_body_jq.find("span.percent").css("color",percent_color_hex);
	   		panel_body_jq.find("small").css("color",quiz_cnt_color_hex);

	   	});
	}
}

// TEST
// var timer = _dates.get_timer_in_seconds();


// TODO
// 1. php utils 파일들 inc로 이전. 기능별로 나눌 것.
// 2. db query시 사용한 파라미터를 feedback manager로 넘기는 방법 개선
// 3. param manager를 constant param - array, set을 이용, 확장 가능한 형태로! / variable param names

</script>
</body>
</html>


