<?php
	// @ common setting
	include_once("../../common.inc");

	$preprocessor = 
	new TitanPreprocessor(
		// $mysql_interface=null
		$mysql_interface
		// $permission_arr=null / null for everyone
		, array(
			$param->USER_PERMISSION_ADMIN			// Admin(모든거, Quota 설정)
			, $param->USER_PERMISSION_EMPLOYEE			// Admin(모든거, Quota 설정)
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

	// USER STATS
	$user_info_list = $mysql_interface->get_user_info_list($PROPS);
	$PROPS->user_info_list = $user_info_list;

	// $user_list = $mysql_interface->select_user_list($PROPS);
	// $PROPS->user_list = $user_list;

	// • 날짜별로 몇 문제 풀었는지
	// • 카테고리별 푼 문제수, 정답률
	// • 그동안 신고 마킹한 문제들 로그 리스트
	// • 총 누적 벌이

	// @ required
	$PROPS->{$param->QUERY_FEEDBACK} = $mysql_interface->get_feedback();
	$mysql_interface->close();

?>



<html>
<head>
<?php
	// @ required
	ViewRenderer::render($PROPS->HEAD_FILE_PATH,$PROPS->HEAD_VIEW_RENDER_VAR_ARR_FORCE_PC_VIEW);
?>
</head>



<body role="document">

	<!-- nav begins -->
	<?php

	$view_render_var_arr = 
	array(
		"[__USER_NAME__]"=>$PROPS->{$param->USER_NICKNAME_N_PERMISSION}
		, "[__ACTIVE_USERS__]"=>"active"
		, "[__SERVICE_ROOT__]"=>$PROPS->SERVICE_ROOT_PATH
	);
	ViewRenderer::render($PROPS->NAV_FILE_PATH,$view_render_var_arr);

	?>

	<div id="container" class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main" style="margin-top:15px;">

	<!-- 3. USER LIST -->
	<?php
	echo "<div class=\"panel panel-primary\">";
		echo "<div id=\"body_user_list\" class=\"panel-body\">";

// CONTROL BAR
echo "<ul class=\"list-group\">";
	echo "<li class=\"list-group-item\" style=\"padding-bottom:44px;background-color:#f7f7f7;\">";

			// COMBO BOX - REGION
			$COMBO_BOX_REGION = $PROPS->{$param->COMBO_BOX_SET}->{$param->COMBO_BOX_REGION};
			$HTML_TAG = $COMBO_BOX_REGION->{ComboBoxManager::$HTML_TAG};
			echo "$HTML_TAG";

	echo "</li>";
echo "</ul>";
// CONTROL BAR					
	


				foreach($user_info_list as $user_info) {

					if(is_null($user_info->user)) {
						continue;
					}

					$offset_str = 7;

					// fb id
					$__user_id = $user_info->user->__id;
					$__email = $user_info->user->__email;

					$__user_fb_id = $user_info->user->__fb_id;
					$__user_fb_id_short = "";
					if(!empty($__user_fb_id)) {
						$strlen_fb_id = strlen($__user_fb_id);
						$head_fb_id = substr($__user_fb_id, 0, $offset_str);
						$tail_fb_id = substr($__user_fb_id, ($strlen_fb_id-$offset_str), $offset_str);
						$__user_fb_id_short = $head_fb_id."...".$tail_fb_id;
					}


					$__user_google_id = $user_info->user->__google_id;
					$__user_google_id_short ="";
					if(!empty($__user_google_id)) {
						$strlen_google_id = strlen($__user_google_id);
						$head_google_id = substr($__user_google_id, 0, $offset_str);
						$tail_google_id = substr($__user_google_id, ($strlen_google_id-$offset_str), $offset_str);
						$__user_google_id_short = $head_google_id."...".$tail_google_id;
					}


					// user name
					$__user_first_name = $user_info->user->__first_name;
					$__user_last_name = $user_info->user->__last_name;
					$__user_nickname = $user_info->user->__nickname;

					// __user_status
					$__user_status = $user_info->user->__status;
					// __user_permission
					$__user_permission = $user_info->user->__permission;
					// __user_quota
					$__user_quota = $user_info->user->__quota;

					$__user_category_access = $user_info->user->__category_access;
					$category_access_arr = array();
					if(!empty($__user_category_access)) {
						$category_access_arr = explode(",",$__user_category_access);	
					}

					// registered date
					$__user_date_created = $user_info->user->__date_created;
					// last log in
					$__user_date_updated = $user_info->user->__date_updated;
					// quiz solved cnt
					$__quiz_solved_total_cnt = $user_info->quiz->quiz_solved_total_cnt;
					// payment
					$__payment = $param->QUIZ_COST * $__quiz_solved_total_cnt;

				// echo "<div class=\"panel panel-default\"><div class=\"panel-body\">";
				echo "<table id=\"user_table\" __user_id=\"$__user_id\" class=\"table table-bordered table-striped\">";
					echo "<thead>";
					echo "<tr>";

					$TABLE_COLUMN_USER_NAME = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_USER_NAME;
					$TABLE_COLUMN_USER_NICKNAME = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_USER_NICKNAME;
					$TABLE_COLUMN_USER_TIME_LAST_LOG_IN = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_USER_TIME_LAST_LOG_IN;
					$TABLE_COLUMN_USER_TIME_SIGN_UP = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_USER_TIME_SIGN_UP;

					// echo 		"<th>#</th>";
					echo 		"<th>Facebook ID</th>";
					echo 		"<th>Google ID</th>";
					echo 		"<th>$TABLE_COLUMN_USER_NAME</th>";
					echo 		"<th>$TABLE_COLUMN_USER_NICKNAME</th>";
					echo 		"<th><small>$TABLE_COLUMN_USER_TIME_LAST_LOG_IN<br/>$TABLE_COLUMN_USER_TIME_SIGN_UP<br/></small></th>";

					echo "</tr>"; 
					echo "</thead>";
					echo "<tbody>";						
					
						
					echo 	"<tr>";

					if(empty($__user_fb_id)) {
						
						// 페이스북 계정이 비어있는 경우, 직접 입력할 수 있도록 입력창을 제공
						// FACEBOOK ID - INIT
						echo 		"<td style=\"width:20%;\">";
						echo 			"<div id=\"facebook_id_input_group\" class=\"input-group\">";
						echo 				"<input __fb_id=\"$__user_fb_id\" __google_id=\"$__user_google_id\" type=\"text\" class=\"form-control\" placeholder=\"Type FBUID\" value=\"$__user_fb_id\">";
						echo 				"<span class=\"input-group-btn\">";
						echo 					"<button id=\"btn_nickname_update\" class=\"btn btn-default\" type=\"button\">OK</button>";
						echo 				"</span>";
						echo 			"</div>";
						echo 		"</td>";
						// FACEBOOK ID - DONE

					} else {
						// 페이스북 계정이 있다면 수정 불가. 화면에 노출.
						echo 		"<td><small>$__user_fb_id</small></td>";
					}
					
					if(empty($__user_google_id)) {
						// 구글 계정 및 메일 계정이 비어있는 경우, 직접 입력할 수 있도록 입력창을 제공
						// EMAIL - INIT
						echo 		"<td style=\"width:20%;\">";
						echo 			"<div id=\"email_input_group\" class=\"input-group\">";
						echo 				"<input __fb_id=\"$__user_fb_id\" __google_id=\"$__user_google_id\" type=\"text\" class=\"form-control\" placeholder=\"Type GMAIL ID\" value=\"$__email\">";
						echo 				"<span class=\"input-group-btn\">";
						echo 					"<button id=\"btn_nickname_update\" class=\"btn btn-default\" type=\"button\">OK</button>";
						echo 				"</span>";
						echo 			"</div>";
						echo 		"</td>";
						// EMAIL - DONE
					} else {
						// 구글 계정이 있다면 수정 불가. 화면에 노출.
						echo 		"<td><small>$__email</small></td>";
					}

					echo 		"<td>$__user_first_name $__user_last_name</td>";

					// NICKNAME - INIT
					echo 		"<td style=\"width:20%;\">";
					echo 			"<div id=\"nickname_input_group\" class=\"input-group\">";
					echo 				"<input __fb_id=\"$__user_fb_id\" __google_id=\"$__user_google_id\" type=\"text\" class=\"form-control\" placeholder=\"Type nickname\" value=\"$__user_nickname\">";
					echo 				"<span class=\"input-group-btn\">";
					echo 					"<button id=\"btn_nickname_update\" class=\"btn btn-default\" type=\"button\">OK</button>";
					echo 				"</span>";
					echo 			"</div>";
					echo 		"</td>";
					// NICKNAME - DONE

					echo 		"<td><small>$__user_date_updated<br/>$__user_date_created</small></td>";

					echo 	"</tr>";

					// extra user properties


					echo "<tr>";

					$TABLE_COLUMN_USER_PAYMENT = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_USER_PAYMENT;
					$TABLE_COLUMN_USER_STATUS = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_USER_STATUS;
					$TABLE_COLUMN_USER_PERMISSION = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_USER_PERMISSION;
					$TABLE_COLUMN_USER_QUIZ_QUOTA = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_USER_QUIZ_QUOTA;
					$TABLE_COLUMN_QUIZ_SOLVED_CNT = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_QUIZ_SOLVED_CNT;

					echo 		"<th>$TABLE_COLUMN_USER_PAYMENT</th>";
					echo 		"<th>$TABLE_COLUMN_USER_STATUS</th>";
					echo 		"<th>$TABLE_COLUMN_USER_PERMISSION</th>";
					echo 		"<th>$TABLE_COLUMN_USER_QUIZ_QUOTA</th>";
					echo 		"<th>$TABLE_COLUMN_QUIZ_SOLVED_CNT</th>";

					echo "</tr>";
					echo "<tr>";

					// user payment
					echo 		"<td>&#8361; $__payment</td>";

					// user status
					echo "<td><select id=\"user_status\" user_id=\"$__user_id\" fb_user_id=\"$__user_fb_id\" google_user_id=\"$__user_google_id\" class=\"form-control\" name=\"list_search_tab\" id=\"list_search_tab\">";

						if(strcmp($__user_status,$param->USER_STATUS_AVAILABLE) == 0) {
							echo "<option value=\"$param->USER_STATUS_AVAILABLE\" selected>AVAILABLE</option>";
							echo "<option value=\"$param->USER_STATUS_NOT_IN_ACTION\">NOT_IN_ACTION</option>";
						} else if(strcmp($__user_status,$param->USER_STATUS_NOT_IN_ACTION) == 0) {
							echo "<option value=\"$param->USER_STATUS_AVAILABLE\">AVAILABLE</option>";
							echo "<option value=\"$param->USER_STATUS_NOT_IN_ACTION\" selected>NOT_IN_ACTION</option>";
						}

					echo "</select></td>";

					// employer - manager는 자신 및 다른사람을 admin으로 등업 혹은 admin에서 내릴 수 없다. 
					$attr_disabled = "";
					$user_permission_arr = $param->get_user_permission_arr();
					if(strcmp($PROPS->USER_INFO->__permission, $param->USER_PERMISSION_EMPLOYEE) == 0) {
						if(strcmp($__user_permission, $param->USER_PERMISSION_ADMIN) == 0) {
							// admin 유저의 퍼미션은 제어 불가.
							$attr_disabled = "disabled";
						} else {
							// 그 이외의 유저들은 퍼미션 항목에서 ADMIN에 제외된다.
							$user_permission_arr_no_admin = array();
							foreach($user_permission_arr AS $user_permission) {
								if(strcmp($user_permission, $param->USER_PERMISSION_ADMIN) == 0) {
									continue;
								}
								array_push($user_permission_arr_no_admin, $user_permission);
							}	

							$user_permission_arr = $user_permission_arr_no_admin;
						}
					}

					// user permission
					echo "<td><select id=\"user_permission\" user_id=\"$__user_id\" fb_user_id=\"$__user_fb_id\" google_user_id=\"$__user_google_id\" class=\"form-control\" name=\"list_search_tab\" id=\"list_search_tab\" $attr_disabled>";

						
						foreach($user_permission_arr AS $user_permission) {

							$user_permission_code = $param->get_permission_code($user_permission);

							if(strcmp($user_permission, $__user_permission) == 0) {
								echo "<option value=\"$user_permission\" selected>$user_permission_code</option>";	
							} else {
								echo "<option value=\"$user_permission\">$user_permission_code</option>";	
							}

						}
					echo "</select></td>";



					// user quota
					echo "<td><select id=\"user_quota\" user_id=\"$__user_id\" fb_user_id=\"$__user_fb_id\" google_user_id=\"$__user_google_id\" class=\"form-control\" name=\"list_search_tab\" id=\"list_search_tab\">";

						$cur_quota_array = $param->get_quota_array();
						foreach($cur_quota_array AS $cur_quota) {

							if(intval($cur_quota) == intval($__user_quota)) {
								echo "<option value=\"$cur_quota\" selected>$cur_quota</option>";	
							} else {
								echo "<option value=\"$cur_quota\">$cur_quota</option>";	
							}

						}

					echo "</select></td>"; 

					echo 		"<td>$__quiz_solved_total_cnt</td>";

					echo "</tr>";








					// seperate category by 6
					$divider = 5;
					$category_group_array = array();
					$category_group=null;
					for($idx = 0; $idx < count($PROPS->QUIZ_CATEGORY_ARR); $idx++) {

						$quiz_category = $PROPS->QUIZ_CATEGORY_ARR[$idx];

						if($idx % $divider == 0) {
							array_push($category_group_array, $category_group);
							$category_group = array();
						}

						// (샌드박스),(전체) 카테고리를 제외한 모든 카테고리를 보여줍니다.
						$category_sandbox = $param->QUIZ_CATEGORY_X_SANDBOX;
						$category_all = $param->QUIZ_CATEGORY_ALL_CATEGORY;

						if( strcmp($quiz_category, $category_sandbox) != 0 && 
							strcmp($quiz_category, $category_all) != 0) {

							array_push($category_group, $quiz_category);	
						}

						if($idx == (count($PROPS->QUIZ_CATEGORY_ARR) - 1)) {
							array_push($category_group_array, $category_group);
						}

					}



					for($idx = 0; $idx < count($category_group_array); $idx++) {

						// Draw each row

						$category_group = $category_group_array[$idx];
						// SET TITLE
						echo "<tr>";

						$toggle_tag = ""
						. "<div id=\"toggle_container\" style=\"float:right;\">"
							. "<input id=\"toggle_input\" region=\"$PROPS->QUIZ_REGION\" language=\"$PROPS->QUIZ_LANGUAGE\" fb_user_id=\"$__user_fb_id\" google_user_id=\"$__user_google_id\" user_id=\"$__user_id\" category=\"\$category\" type=\"checkbox\" data-toggle=\"toggle\" data-on=\"ON\" data-off=\"OFF\" \$checked>"
						. "</div>"
						;

						for($inner_idx = 0; $inner_idx < count($category_group); $inner_idx++) {

							// Draw each column

							$quiz_category = $category_group[$inner_idx];

							$quiz_category_code = 
							$param->get_category_code(
								// $region=""
								$PROPS->QUIZ_REGION
								//, $language=""
								, $PROPS->QUIZ_LANGUAGE
								//, $category=""
								, $quiz_category
							);
							// REMOVE ME
							// $quiz_category_code = $PROPS->QUIZ_CATEGORY_CODE_SET->{$quiz_category};

							$pattern = '/\$category/';
							$replacement = $quiz_category;
							$toggle_tag_updated = preg_replace($pattern, $replacement, $toggle_tag);

							// CHECK - TOGGLE UPDATE
							$user_access_token = $param->get_user_access_token($PROPS->QUIZ_REGION, $quiz_category);
							$has_access = false;
							for($k = 0; $k < count($category_access_arr); $k++) {
								$cur_category_access = $category_access_arr[$k];

								if(strcmp($cur_category_access, $user_access_token) == 0) {
									$has_access = true;
									break;
								}
							}
							$pattern = '/\$checked/';
							$replacement = ($has_access)?"checked":"";
							$toggle_tag_updated = preg_replace($pattern, $replacement, $toggle_tag_updated);

							echo "<th>$quiz_category_code $toggle_tag_updated</th>";

							if($inner_idx == (count($category_group) - 1)) {

								// 카테고리 그룹의 마지막에 All on / All off 버튼을 추가합니다.
								// 문제 : 카테고리 그룹에 남는 공간이 없다면 버튼이 그려지지 않습니다.
								// 제안 : 모든 카테고리를 이 버튼에 할당한다면 어떨까?
								$left_cnt = $divider - count($category_group);
								for($j = 0; $j < $left_cnt; $j++) {

									echo "<th>";
									echo "<button id=\"on_category_access\" type=\"button\" class=\"btn btn-default\" style=\"margin-right:10px;\"  region=\"$PROPS->QUIZ_REGION\" language=\"$PROPS->QUIZ_LANGUAGE\" fb_user_id=\"$__user_fb_id\" google_user_id=\"$__user_google_id\" user_id=\"$__user_id\">";
										echo "<strong>All on</strong>";
									echo "</button>";
									echo "<button id=\"off_category_access\" type=\"button\" class=\"btn btn-default\" region=\"$PROPS->QUIZ_REGION\" language=\"$PROPS->QUIZ_LANGUAGE\" fb_user_id=\"$__user_fb_id\" google_user_id=\"$__user_google_id\" user_id=\"$__user_id\">";
										echo "<strong>All off</strong>";
									echo "</button>";
									echo "</th>";

									// All on / All off 1개씩만 그립니다.
									break;

								}
								
							} // end if

						} // end inner for



						echo "</tr>"; 
						echo "<tr>";

						// SET CONTENT
						for($inner_idx = 0; $inner_idx < count($category_group); $inner_idx++) {

							$quiz_category = $category_group[$inner_idx];
							$quiz_category_stat = $user_info->quiz->category->{$quiz_category};

							$quiz_cnt_msg = $quiz_category_stat->quiz_cnt_msg;
							$ratio = $quiz_category_stat->ratio;

							$msg = "$quiz_cnt_msg($ratio)";
							echo 		"<td>$msg</td>";
							
							// draw blank table element
							if($inner_idx == (count($category_group) - 1)) {

								$left_cnt = $divider - count($category_group);
								for($j = 0; $j < $left_cnt; $j++) {
									echo 		"<td></td>";
								}
								
							} // end if
						} // end inner for

						echo "</tr>";
					}






					// user feed back
					$__user_feedback_like_cnt = $user_info->quiz->feedback->__user_feedback_like_cnt;
					$__user_feedback_dislike_cnt = $user_info->quiz->feedback->__user_feedback_dislike_cnt;
					$__user_feedback_error_cnt = $user_info->quiz->feedback->__user_feedback_error_cnt;
					$__user_feedback_too_easy_cnt = $user_info->quiz->feedback->__user_feedback_too_easy_cnt;
					$__user_feedback_too_hard_cnt = $user_info->quiz->feedback->__user_feedback_too_hard_cnt;
					
					echo "<tr>";

					$TABLE_COLUMN_FEEDBACK_LIKE = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_FEEDBACK_LIKE;
					$TABLE_COLUMN_FEEDBACK_DISLIKE = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_FEEDBACK_DISLIKE;
					$TABLE_COLUMN_FEEDBACK_ERROR = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_FEEDBACK_ERROR;
					$TABLE_COLUMN_FEEDBACK_TOO_HARD = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_FEEDBACK_TOO_HARD;
					$TABLE_COLUMN_FEEDBACK_TOO_EASY = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_FEEDBACK_TOO_EASY;

					echo 		"<th>$TABLE_COLUMN_FEEDBACK_LIKE</th>";
					echo 		"<th>$TABLE_COLUMN_FEEDBACK_DISLIKE</th>";
					echo 		"<th>$TABLE_COLUMN_FEEDBACK_ERROR</th>";
					echo 		"<th>$TABLE_COLUMN_FEEDBACK_TOO_HARD</th>";
					echo 		"<th>$TABLE_COLUMN_FEEDBACK_TOO_EASY</th>";

					echo "</tr>"; 
					echo "<tr>";

					echo 		"<td>$__user_feedback_like_cnt</td>";
					echo 		"<td>$__user_feedback_dislike_cnt</td>";
					echo 		"<td>$__user_feedback_error_cnt</td>";
					echo 		"<td>$__user_feedback_too_easy_cnt</td>";
					echo 		"<td>$__user_feedback_too_hard_cnt</td>";

					echo 	"</tr>";

					echo "</tbody>";
				echo "</table>";
				// echo "</div></div>";

				} // end foreach
					echo "</div>";
				echo "</div>";			

			?>
	</div>

<script>

// php to javascript sample
var PROPS = <?php echo json_encode($PROPS);?>;

// set event
var header_user_list_jq = $("div#header_user_list");
var body_user_list_jq = $("div#body_user_list");

header_user_list_jq.click(function(e){

	if(body_user_list_jq.is(":visible")) {
		body_user_list_jq.hide();
	} else {
		body_user_list_jq.show();
	}

});

var user_table_jq_list = $("table#user_table");

// user_status / user_permission / user_quota
var COMBO_BOX_ID = PROPS.COMBO_BOX_SET.COMBO_BOX_REGION.COMBO_BOX_ID;
var select_region_jq = $("select#" + COMBO_BOX_ID);
select_region_jq.change(function(){
	var selected_value = $(this).val();

	_link.refresh_post(
		_param
		.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
		.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)
		.get(PROPS.PARAM_SET.QUIZ_REGION, selected_value)
	);

});


var select_user_status_jq = $("select#user_status");
select_user_status_jq.change(function(){
	var selected_value = $(this).val();

	var _self_jq = $(this);
	var fb_user_id = _self_jq.attr("fb_user_id");
	var google_user_id = _self_jq.attr("google_user_id");

	var request_param_obj = 
	_param
	.get(PROPS.PARAM_SET.EVENT_TYPE, PROPS.PARAM_SET.EVENT_TYPE_USER_INFO_UPDATE)
	.get(PROPS.PARAM_SET.USER_STATUS, selected_value)
	.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, fb_user_id)
	.get(PROPS.PARAM_SET.GOOGLE_USER_ID, google_user_id)
	;

	console.log("select_user_status_jq.change / request_param_obj ::: ",request_param_obj);

	var _self = this;
	_ajax.post(
		// _url
		_link.get_link(_link.API_UPDATE_USER)
		// _param_obj
		,request_param_obj
		// _delegate_after_job_done
		,_obj.get_delegate(
			// delegate_func
			function(data){

				console.log("data :: ",data);

			},
			// delegate_scope
			this
		)
	); // ajax done.

});
var select_user_permission_jq = $("select#user_permission");
select_user_permission_jq.change(function(){
	var selected_value = $(this).val();

	var _self_jq = $(this);
	var fb_user_id = _self_jq.attr("fb_user_id");
	var google_user_id = _self_jq.attr("google_user_id");

	// Employee (Manager) 매니저인 경우, Admin으로 권한 승급은 불가.
	if( PROPS.USER_INFO.__permission === PROPS.PARAM_SET.USER_PERMISSION_EMPLOYEE && 
		selected_value === PROPS.PARAM_SET.USER_PERMISSION_ADMIN) {

		console.log("Employee (Manager) 매니저인 경우, Admin으로 권한 승급은 불가.");
		return;
	}

	var request_param_obj = 
	_param
	.get(PROPS.PARAM_SET.EVENT_TYPE, PROPS.PARAM_SET.EVENT_TYPE_USER_INFO_UPDATE)
	.get(PROPS.PARAM_SET.USER_PERMISSION, selected_value)
	.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, fb_user_id)
	.get(PROPS.PARAM_SET.GOOGLE_USER_ID, google_user_id)
	;


	var _self = this;
	_ajax.post(
		// _url
		_link.get_link(_link.API_UPDATE_USER)
		// _param_obj
		,request_param_obj
		// _delegate_after_job_done
		,_obj.get_delegate(
			// delegate_func
			function(data){

				if(!data.SUCCESS) {
					console.log("Error - Permission update has been failed! - #575");
					return;
				}

				if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
					console.log("data :: ",data);	
				}

				if(data.USER_INFO_UPDATED.__permission !== PROPS.PARAM_SET.USER_PERMISSION_ADMIN) {
					return;
				}


				// 운영자 업데이트인 경우, 전체 카테고리에 접근하도록 변경.
				var request_param_obj = 
				_param
				.get(PROPS.PARAM_SET.EVENT_TYPE, PROPS.PARAM_SET.EVENT_TYPE_ADMIN_CATEGORY_ACCESS_UPDATE_TOGGLE)
				.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, data.USER_INFO_UPDATED.__fb_id)
				.get(PROPS.PARAM_SET.GOOGLE_USER_ID, data.USER_INFO_UPDATED.__google_id)
				;

				_ajax.post(
					// _url
					_link.get_link(_link.API_UPDATE_USER)
					// _param_obj
					,request_param_obj
					// _delegate_after_job_done
					,_obj.get_delegate(
						// delegate_func
						function(data){

							if(!data.SUCCESS) {
								console.log("Error - Admin Category Accees update has been failed! - #606");
								return;
							}

							if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
								console.log("data :: ",data);	
							}

							// 모든 category toggle을 켜준다.
							is_lock_to_toggle_category_access = true;

							var parent_jq = _self_jq.parent().parent().parent();
							var btn_toggle_ele_arr = parent_jq.find("input#toggle_input");
							for( var idx=0; idx < btn_toggle_ele_arr.length;idx++) {
								var btn_toggle_ele = btn_toggle_ele_arr[idx];
								var btn_toggle_jq = $(btn_toggle_ele);

								// 현재 버튼의 상태는 어떻게 아는지?
								btn_toggle_jq.bootstrapToggle('on');
							}

							// 3초 뒤에 개별 토글 가능하도록 변경
							setTimeout(function(){ is_lock_to_toggle_category_access = false; }, 3000);

						},
						// delegate_scope
						this
					)
				); // ajax done.

			},
			// delegate_scope
			this
		)
	); // ajax done.


});
var select_user_quota_jq = $("select#user_quota");
select_user_quota_jq.change(function(){

	var selected_value = $(this).val();

	var _self_jq = $(this);
	var fb_user_id = _self_jq.attr("fb_user_id");
	var google_user_id = _self_jq.attr("google_user_id");

	var request_param_obj = 
	_param
	.get(PROPS.PARAM_SET.EVENT_TYPE, PROPS.PARAM_SET.EVENT_TYPE_USER_INFO_UPDATE)
	.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, fb_user_id)
	.get(PROPS.PARAM_SET.GOOGLE_USER_ID, google_user_id)
	.get(PROPS.PARAM_SET.USER_QUOTA, selected_value) // wonder.jung - 0을 null로 처리하는 이슈 있음. 
	;

	var _self = this;
	_ajax.post(
		// _url
		_link.get_link(_link.API_UPDATE_USER)
		// _param_obj
		,request_param_obj
		// _delegate_after_job_done
		,_obj.get_delegate(
			// delegate_func
			function(data){

				console.log("data :: ",data);

			},
			// delegate_scope
			this
		)
	); // ajax done.


});



// language_list / category_list / user_list
var btn_show_quiz_user_solved_list = $("button#show_quiz_user_solved_list");
btn_show_quiz_user_solved_list.click(function(e){

	var select_language_list_jq = $("select#language_list");
	var language = select_language_list_jq.val();
	var select_category_list_jq = $("select#category_list");
	var category = select_category_list_jq.val();
	var select_user_list_jq = $("select#user_list");
	var user_id = select_user_list_jq.val();

	var start_date = datepicker_start_date_jq.val();
	var end_date = datepicker_end_date_jq.val();

	var _param_obj = 
	_param
	.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
	.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)
	;

	_link.refresh_post(
		_param_obj
	);

});

// CATEGORY ACCESS

// 1. 유저 리스트에서 카테고리에 checkbox를 넣어둠.
// 1-1. 유저가 체크를 변경할 때마다 서버 통신으로 데이터 업데이트.

// ex) KOR_KOR.G_COMMON : ${region}_${language}_${category}

for(var idx=0; idx < user_table_jq_list.length; idx++) {

	var user_table_jq = $(user_table_jq_list[idx]);
	var toggle_jq_arr = user_table_jq.find("input#toggle_input");

	toggle_jq_arr.change(function(e){

		if(is_lock_to_toggle_category_access) {
			return;
		}

		var _self = $(this);
		var is_off = _self.parent().hasClass("off");

		var region = _self.attr("region");
		var language = _self.attr("language");

		var user_id = _self.attr("user_id");
		var fb_user_id = _self.attr("fb_user_id");
		var google_user_id = _self.attr("google_user_id");

		var category = _self.attr("category");

		var USER_ACCESS_STATUS = PROPS.PARAM_SET.USER_ACCESS_STATUS_ON;
		if(is_off) {
			USER_ACCESS_STATUS = PROPS.PARAM_SET.USER_ACCESS_STATUS_OFF;
		}

		var request_param_obj = 
		_param
		.get(PROPS.PARAM_SET.EVENT_TYPE, PROPS.PARAM_SET.EVENT_TYPE_USER_CATEGORY_ACCESS_UPDATE)
		.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, fb_user_id)
		.get(PROPS.PARAM_SET.GOOGLE_USER_ID, google_user_id)
		.get(PROPS.PARAM_SET.QUIZ_REGION, region)
		.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, language)
		.get(PROPS.PARAM_SET.QUIZ_CATEGORY, category)
		.get(PROPS.PARAM_SET.USER_ACCESS_STATUS, USER_ACCESS_STATUS)
		;

		var _self = this;
		_ajax.post(
			// _url
			_link.get_link(_link.API_UPDATE_USER)
			// _param_obj
			,request_param_obj
			// _delegate_after_job_done
			,_obj.get_delegate(
				// delegate_func
				function(data){

					if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
						console.log("data :: ",data);	
					}

				},
				// delegate_scope
				this
			)
		); // ajax done.

	});


}

var on_save_nickname = function(input_jq) {

	if(input_jq == null) {
		return;
	}

	var cur_nickname = input_jq.val();
	var __google_id = input_jq.attr("__google_id");
	var __fb_id = input_jq.attr("__fb_id");

	if(_v.is_not_valid_str(cur_nickname)) {
		console.log("Nickname should not be empty!\nPlease check again.");
		input_jq.focus();
		return;
	}

	// unique check.
	// 현재 화면의 모든 닉네임과 비교.
	var input_nickname_jq_arr = $("div#nickname_input_group input");
	for(var idx=0; idx < input_nickname_jq_arr.length; idx++) {
		input_nickname_ele = input_nickname_jq_arr[idx];
		input_nickname_jq = $(input_nickname_ele);

		var other_fb_id = input_nickname_jq.attr("__fb_id");
		if(other_fb_id === __fb_id) {
			// 자기 자신입니다. 검사하지 않습니다
			continue;
		}
		var other_google_id = input_nickname_jq.attr("__google_id");
		if(other_google_id === __google_id) {
			// 자기 자신입니다. 검사하지 않습니다
			continue;
		}

		var other_nickname = input_nickname_jq.val();

		if(_v.is_not_valid_str(other_nickname)) {
			continue;
		}

		if(cur_nickname === other_nickname) {
			// TODO - POPOVER ALERT
			console.log("Nickname is not unique!\nPlease check again.");
			input_jq.focus();
			return;
		}
	}

	var request_param_obj = 
	_param
	.get(PROPS.PARAM_SET.EVENT_TYPE, PROPS.PARAM_SET.EVENT_TYPE_USER_INFO_UPDATE)
	.get(PROPS.PARAM_SET.USER_NICKNAME, cur_nickname)
	.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, __fb_id)
	.get(PROPS.PARAM_SET.GOOGLE_USER_ID, __google_id)
	;

	var _self = this;
	_ajax.post(
		// _url
		_link.get_link(_link.API_UPDATE_USER)
		// _param_obj
		,request_param_obj
		// _delegate_after_job_done
		,_obj.get_delegate(
			// delegate_func
			function(data){

				console.log("data :: ",data);

				if(data != null && data.updated_user_obj != null) {
					console.log("Nickname updated!");
					return;
				}

				if(parseInt(data.USER_INFO_UPDATED.__id) === parseInt(PROPS.USER_INFO.__id)) {
					console.log("닉네임 변경 성공! 자신의 이름일 경우, 화면 상단의 자신의 닉네임을 변경해줍니다1.");

					var msg = 
					"<USER_NICKNAME> (<USER_PERMISSION_CODE>)"
					.replace(/\<USER_NICKNAME\>/gi, data.USER_INFO_UPDATED.__nickname)
					.replace(/\<USER_PERMISSION_CODE\>/gi, PROPS.USER_PERMISSION_CODE)
					;

					var log_in_user_name_jq = $("span#log_in_user_nickname");
					log_in_user_name_jq.html(msg);
				}

			},
			// delegate_scope
			this
		)
	); // ajax done.	

}
var on_save_facebook_id = function(google_id, facebook_id) {

	var request_param_obj = 
	_param
	.get(PROPS.PARAM_SET.EVENT_TYPE, PROPS.PARAM_SET.EVENT_TYPE_UPDATE_USER_FACEBOOK_ID)
	.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, facebook_id)
	.get(PROPS.PARAM_SET.GOOGLE_USER_ID, google_id)
	;

	var _self = this;
	_ajax.post(
		// _url
		_link.get_link(_link.API_UPDATE_USER)
		// _param_obj
		,request_param_obj
		// _delegate_after_job_done
		,_obj.get_delegate(
			// delegate_func
			function(data){

				if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
					console.log("data :: ",data);	
				}

				if(!data.SUCCESS) {
					return;
				}

				_link.refresh_post(
					_param
					.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
					.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)
					.get(PROPS.PARAM_SET.QUIZ_REGION, PROPS.QUIZ_REGION)
				);
			},
			// delegate_scope
			this
		)
	); // ajax done.	

}
var input_facebook_id_jq = $("div#facebook_id_input_group").find("input");
input_facebook_id_jq.keyup(function( event ) {

	var _self_jq = $(this);


	if(event.which === 13) {
		// ENTER KEY
		var google_id = _self_jq.attr("__google_id");
		if(_v.is_not_valid_str(google_id)) {
			return;
		}

		var facebook_id = _self_jq.val();
		if(_v.is_not_valid_str(facebook_id)) {
			return;
		}

		on_save_facebook_id(google_id, facebook_id);
	}

});
var input_facebook_id_btn_ok_jq = $("div#facebook_id_input_group").find("button");
input_facebook_id_btn_ok_jq.click(function(){

	var _self_jq = $(this);
	var input_jq = _self_jq.parent().parent().find("input");

	var google_id = input_jq.attr("__google_id");
	if(_v.is_not_valid_str(google_id)) {
		return;
	}

	var facebook_id = input_jq.val();
	if(_v.is_not_valid_str(facebook_id)) {
		return;
	}

	on_save_facebook_id(google_id, facebook_id);

});

var on_save_email_id = function(facebook_id, user_email) {

	var request_param_obj = 
	_param
	.get(PROPS.PARAM_SET.EVENT_TYPE, PROPS.PARAM_SET.EVENT_TYPE_UPDATE_USER_EMAIL)
	.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, facebook_id)
	.get(PROPS.PARAM_SET.USER_EMAIL, user_email)
	;

	var _self = this;
	_ajax.post(
		// _url
		_link.get_link(_link.API_UPDATE_USER)
		// _param_obj
		,request_param_obj
		// _delegate_after_job_done
		,_obj.get_delegate(
			// delegate_func
			function(data){

				if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
					console.log("data :: ",data);	
				}

				if(!data.SUCCESS) {
					return;
				}

				// TEST
				return;

				_link.refresh_post(
					_param
					.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
					.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)
					.get(PROPS.PARAM_SET.QUIZ_REGION, PROPS.QUIZ_REGION)
				);
			},
			// delegate_scope
			this
		)
	); // ajax done.

}
var input_email_jq = $("div#email_input_group").find("input");
input_email_jq.keyup(function( event ) {

	var _self_jq = $(this);

	if(event.which === 13) {
		var new_email = _self_jq.val();
		var facebook_id = _self_jq.attr("__fb_id");

		on_save_email_id(facebook_id, new_email);
	}

});
var input_email_btn_ok_jq = $("div#email_input_group").find("button");
input_email_btn_ok_jq.click(function(e) {

	var _self_jq = $(this);
	var input_jq = _self_jq.parent().parent().find("input");
	var new_email = input_jq.val();
	var facebook_id = input_jq.attr("__fb_id");

	if(_v.is_not_valid_str(new_email)) {
		return;
	}
	if(_v.is_not_valid_str(facebook_id)) {
		return;
	}

	on_save_email_id(facebook_id, new_email);

});
var input_nickname_jq = $("div#nickname_input_group").find("input");
input_nickname_jq.keyup(function( event ) {

	var _self_jq = $(this);

	if(event.which === 13) {
		on_save_nickname(_self_jq);
	}

});
var input_nickname_btn_ok_jq = $("div#nickname_input_group").find("button");
input_nickname_btn_ok_jq.click(function(e){
	console.log("input_nickname_btn_ok_jq.click");

});
var btn_nickname_update_jq_arr = $("div#nickname_input_group").find("button#btn_nickname_update");
btn_nickname_update_jq_arr.click(function(e){

	var _self_jq = $(this);
	var input_jq = _self_jq.parent().parent().find("input");
	on_save_nickname(input_jq);

});




var is_lock_to_toggle_category_access = false;
var btn_off_category_access_jq_arr = $("button#off_category_access");
btn_off_category_access_jq_arr.click(function(e){

	var _self_jq = $(this);

	// wonder.jung

	var region = _self_jq.attr("region");
	var language = _self_jq.attr("language");

	var user_id = _self_jq.attr("user_id");
	var fb_user_id = _self_jq.attr("fb_user_id");
	var google_user_id = _self_jq.attr("google_user_id");

	var USER_ACCESS_STATUS = PROPS.PARAM_SET.USER_ACCESS_STATUS_OFF;

	var request_param_obj = 
	_param
	.get(PROPS.PARAM_SET.EVENT_TYPE, PROPS.PARAM_SET.EVENT_TYPE_USER_CATEGORY_ACCESS_UPDATE_TOGGLE)
	.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, fb_user_id)
	.get(PROPS.PARAM_SET.GOOGLE_USER_ID, google_user_id)
	.get(PROPS.PARAM_SET.QUIZ_REGION, region)
	.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, language)
	.get(PROPS.PARAM_SET.USER_ACCESS_STATUS, USER_ACCESS_STATUS)
	;

	var _self = this;
	_ajax.post(
		// _url
		_link.get_link(_link.API_UPDATE_USER)
		// _param_obj
		,request_param_obj
		// _delegate_after_job_done
		,_obj.get_delegate(
			// delegate_func
			function(data){

				if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
					console.log("data :: ",data);	
				}

				if(data.SUCCESS) {

					is_lock_to_toggle_category_access = true;

					var parent_jq = _self_jq.parent().parent().parent();
					var btn_toggle_ele_arr = parent_jq.find("input#toggle_input");
					for( var idx=0; idx < btn_toggle_ele_arr.length;idx++) {
						var btn_toggle_ele = btn_toggle_ele_arr[idx];
						var btn_toggle_jq = $(btn_toggle_ele);

						// 현재 버튼의 상태는 어떻게 아는지?
						btn_toggle_jq.bootstrapToggle('off');
					}		

					// 3초 뒤에 개별 토글 가능하도록 변경
					setTimeout(function(){ is_lock_to_toggle_category_access = false; }, 3000);

				} else {
					console.log("Error - #846");
				}

			},
			// delegate_scope
			this
		)
	); // ajax done.




});


var btn_on_category_access_jq_arr = $("button#on_category_access");
btn_on_category_access_jq_arr.click(function(e){

	var _self_jq = $(this);

	// ajax로 한꺼번에 모두 업데이트
	// 그 이후 뷰 수정.

	var region = _self_jq.attr("region");
	var language = _self_jq.attr("language");

	var user_id = _self_jq.attr("user_id");
	var fb_user_id = _self_jq.attr("fb_user_id");
	var google_user_id = _self_jq.attr("google_user_id");

	var USER_ACCESS_STATUS = PROPS.PARAM_SET.USER_ACCESS_STATUS_ON;

	var request_param_obj = 
	_param
	.get(PROPS.PARAM_SET.EVENT_TYPE, PROPS.PARAM_SET.EVENT_TYPE_USER_CATEGORY_ACCESS_UPDATE_TOGGLE)
	.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, fb_user_id)
	.get(PROPS.PARAM_SET.GOOGLE_USER_ID, google_user_id)
	.get(PROPS.PARAM_SET.QUIZ_REGION, region)
	.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, language)
	.get(PROPS.PARAM_SET.USER_ACCESS_STATUS, USER_ACCESS_STATUS)
	;

	var _self = this;
	_ajax.post(
		// _url
		_link.get_link(_link.API_UPDATE_USER)
		// _param_obj
		,request_param_obj
		// _delegate_after_job_done
		,_obj.get_delegate(
			// delegate_func
			function(data){

				if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
					console.log("data :: ",data);	
				}

				if(data.SUCCESS) {

					is_lock_to_toggle_category_access = true;

					var parent_jq = _self_jq.parent().parent().parent();
					var btn_toggle_ele_arr = parent_jq.find("input#toggle_input");
					for( var idx=0; idx < btn_toggle_ele_arr.length;idx++) {
						var btn_toggle_ele = btn_toggle_ele_arr[idx];
						var btn_toggle_jq = $(btn_toggle_ele);

						// 현재 버튼의 상태는 어떻게 아는지?
						btn_toggle_jq.bootstrapToggle('on');
					}

					// 3초 뒤에 개별 토글 가능하도록 변경
					setTimeout(function(){ is_lock_to_toggle_category_access = false; }, 3000);

				} else {

					console.log("Error - #910");

				}

			},
			// delegate_scope
			this
		)
	); // ajax done.	



});


</script>
</body>
</html>


