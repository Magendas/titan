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
			, $param->USER_PERMISSION_EMPLOYEE		// Employee(문제,분석,Sim)
			, $param->USER_PERMISSION_USER			// User(문제,분석,Sim)
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

	// Xogames json 상태를 확인해서 이상이 있다면 메시지를 노출.





	$QUIZ_REGION = $PROPS->{$param->QUIZ_REGION};
	$QUIZ_LANGUAGE = $PROPS->{$param->QUIZ_LANGUAGE};

	// 카테고리 샌드박스의 경우에 TARGET_AUTHOR_NAME을 자신의 이름으로 설정합니다.
	if(strcmp($PROPS->QUIZ_CATEGORY, $param->QUIZ_CATEGORY_X_SANDBOX) == 0) {
		$PROPS->{$param->TARGET_AUTHOR_NAME} = $PROPS->USER_INFO->__nickname;
	}

	// PAGINATION - INIT
	$total_cnt = 0;
	if($param->is_default_language($PROPS->QUIZ_REGION, $PROPS->QUIZ_LANGUAGE)) {
		$total_cnt = $mysql_interface->select_quiz_total_cnt($PROPS);
	} else {
		$total_cnt = $mysql_interface->select_quiz_translation_total_cnt($PROPS);
	}
	$preprocessor->set_pagination_total_cnt($total_cnt);
	$PROPS = $preprocessor->get_props();
	$PROPS->total_cnt = $total_cnt;
	// PAGINATION - DONE

	// load quiz list
	if($param->is_default_language($PROPS->QUIZ_REGION, $PROPS->QUIZ_LANGUAGE)) {
		$quiz_list = $mysql_interface->select_quiz_stat_list_by_page($PROPS);
	} else {
		$quiz_list = $mysql_interface->select_quiz_translation_stat_list_by_page($PROPS);
	}
	$PROPS->quiz_list = $quiz_list;

	// category stats
	$quiz_category_stat = $mysql_interface->select_quiz_category_stats($PROPS);
	$PROPS->quiz_category_stat = $quiz_category_stat;

	if(isset($PROPS->QUIZ_CATEGORY) && strcmp($PROPS->QUIZ_CATEGORY, $param->QUIZ_CATEGORY_ALL_CATEGORY) != 0) {
		$category_stat = $quiz_category_stat->{$PROPS->QUIZ_CATEGORY};
	}


	// load quiz list for export
	$prev_page_num = $PROPS->PAGE_NUM;
	$PROPS->PAGE_NUM = 1;
	$prev_row_cnt = $PROPS->ROW_CNT;
	$PROPS->ROW_CNT = 50000;
	if($param->is_default_language($PROPS->QUIZ_REGION, $PROPS->QUIZ_LANGUAGE)) {
		$quiz_list_for_export = $mysql_interface->select_quiz_stat_list_to_export($PROPS);
	} else {
		$quiz_list_for_export = $mysql_interface->select_quiz_translation_stat_list_to_export($PROPS);
	}
	$PROPS->quiz_list_for_export = $quiz_list_for_export;
	// rollback
	$PROPS->PAGE_NUM = $prev_page_num;
	$PROPS->ROW_CNT = $prev_row_cnt;

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
		, "[__QUIZ_FACTORY_COLLASE__]"=>"in"
		, "[__ACTIVE_QUIZ_FACTORY_LIST__]"=>"active"
		, "[__ACTIVE_QUIZ_FACTORY_LIST_STYLE__]"=>"color:#FFFFFF !important;"
		, "[__SERVICE_ROOT__]"=>$PROPS->SERVICE_ROOT_PATH
	);
	ViewRenderer::render($PROPS->NAV_FILE_PATH,$view_render_var_arr);

	?>
	<!-- nav ends -->

	<div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main" style="margin-top:15px;">			
		

		<!-- 4. QUIZ STAT -->
		<?php

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


			echo "<div class=\"panel panel-primary\">";
				echo "<div id=\"body_quiz_progress\" class=\"panel-body\">";

// CONTROL BAR
echo "<ul class=\"list-group\" style=\"margin-bottom: 15px;\">";

	// COMBO BOX SET - INIT
	echo "<li class=\"list-group-item xo_control_box_bg_color\" style=\"padding-bottom:44px;\">";

			// COMBO BOX - REGION
			$COMBO_BOX_REGION = $PROPS->{$param->COMBO_BOX_SET}->{$param->COMBO_BOX_REGION};
			$HTML_TAG = $COMBO_BOX_REGION->{ComboBoxManager::$HTML_TAG};
			echo "$HTML_TAG";

			// COMBO BOX - LANGUAGE
			$COMBO_BOX_LANGUAGE = $PROPS->{$param->COMBO_BOX_SET}->{$param->COMBO_BOX_LANGUAGE};
			$HTML_TAG = $COMBO_BOX_LANGUAGE->{ComboBoxManager::$HTML_TAG};
			echo "$HTML_TAG";

			// COMBO BOX - CATEGORY ALL
			$COMBO_BOX_CATEGORY_ALL = $PROPS->{$param->COMBO_BOX_SET}->{$param->COMBO_BOX_CATEGORY_ALL};
			$HTML_TAG = $COMBO_BOX_CATEGORY_ALL->{ComboBoxManager::$HTML_TAG};
			echo "$HTML_TAG";

			// COMBO BOX - QUIZ STATUS
			$COMBO_BOX_QUIZ_STATUS = $PROPS->{$param->COMBO_BOX_SET}->{$param->COMBO_BOX_QUIZ_STATUS};
			$HTML_TAG = $COMBO_BOX_QUIZ_STATUS->{ComboBoxManager::$HTML_TAG};
			echo "$HTML_TAG";

			// COMBO BOX - ROW CNT
			$COMBO_BOX_ROW_CNT = $PROPS->{$param->COMBO_BOX_SET}->{$param->COMBO_BOX_ROW_CNT};
			$HTML_TAG = $COMBO_BOX_ROW_CNT->{ComboBoxManager::$HTML_TAG};
			echo "$HTML_TAG";

			// BUTTON - SHOW
			$BTN_SHOW = $PROPS->BUTTON_SET->BTN_SHOW;
			$HTML_TAG = $BTN_SHOW->HTML_TAG;
			echo "$HTML_TAG";

			// BUTTON - SHOW
			$BTN_EXPORT = $PROPS->BUTTON_SET->BTN_EXPORT;
			$HTML_TAG = $BTN_EXPORT->HTML_TAG;
			echo "$HTML_TAG";

	echo "</li>";
	// COMBO BOX SET - DONE

	// SEARCH BOX - INIT
	echo "<li class=\"list-group-item xo_control_box_bg_color\" style=\"padding-left:10px;\">";

			// Search
			echo "<div id=\"quiz_search\" class=\"input-group\" style=\"width:30%;\">";
			echo 	"<input type=\"text\" class=\"form-control\" placeholder=\"Type search keyword\" value=\"$PROPS->SEARCH_KEYWORD\">";
			echo 	"<span class=\"input-group-btn\">";
			echo 		"<button id=\"btn_search\" class=\"btn btn-default\" type=\"button\">SEARCH</button>";
			echo 	"</span>";
			echo "</div>";

	echo "</li>";
	// SEARCH BOX - DONE

echo "</ul>";
// CONTROL BAR	

			// 검색어 입력시에는 결과 노출을 위해 통계 표를 보여주지 않습니다.
			$table_display_style="";
			if(!empty($PROPS->SEARCH_KEYWORD)) {
				$table_display_style="display:none;";
			}

			// CATEGORY PROGRESS INIT
			// TABLE
			echo "<table id=\"user_table\" class=\"table table-hover table-bordered table-striped\" style=\"$table_display_style\">";
				echo "<thead>";
				echo "<tr>";

				$TABLE_COLUMN_CATEGORY = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_CATEGORY;

				$TABLE_COLUMN_QUIZ_CNT = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_QUIZ_CNT;
				$TABLE_COLUMN_QUIZ_SOLVED_CNT = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_QUIZ_SOLVED_CNT;
				$TABLE_COLUMN_QUIZ_QUALIFIED_CNT = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_QUIZ_QUALIFIED_CNT;
				$TABLE_COLUMN_QUIZ_CORRECT_RATIO = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_QUIZ_CORRECT_RATIO;

				$TABLE_COLUMN_AVG_TIME_QUALIFIED = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_AVG_TIME_QUALIFIED;
				$TABLE_COLUMN_AVG_TIME_SOLVED = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_AVG_TIME_SOLVED;

				$TABLE_COLUMN_QUIZ_COMMENT_CNT = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_QUIZ_COMMENT_CNT;

				$TABLE_COLUMN_FEEDBACK_LIKE = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_FEEDBACK_LIKE;
				$TABLE_COLUMN_FEEDBACK_DISLIKE = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_FEEDBACK_DISLIKE;
				$TABLE_COLUMN_FEEDBACK_ERROR = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_FEEDBACK_ERROR;
				$TABLE_COLUMN_FEEDBACK_TOO_HARD = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_FEEDBACK_TOO_HARD;
				$TABLE_COLUMN_FEEDBACK_TOO_EASY = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_FEEDBACK_TOO_EASY;
				$TABLE_COLUMN_FEEDBACK_NEED_COMMENT = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_FEEDBACK_NEED_COMMENT;				

				$TABLE_COLUMN_FEEDBACK_FEELING_ALL = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_FEEDBACK_FEELING_ALL;

				echo 		"<th>$TABLE_COLUMN_CATEGORY</th>";
				echo 		"<th>$TABLE_COLUMN_QUIZ_CNT</th>";
				echo 		"<th><small>$TABLE_COLUMN_QUIZ_QUALIFIED_CNT/</small><br/><small>$TABLE_COLUMN_QUIZ_SOLVED_CNT</small><br/><small>($TABLE_COLUMN_QUIZ_CORRECT_RATIO)</small></th>";

				echo 		"<th>$TABLE_COLUMN_AVG_TIME_QUALIFIED</th>";
				echo 		"<th>$TABLE_COLUMN_AVG_TIME_SOLVED</th>";

				echo 		"<th>$TABLE_COLUMN_FEEDBACK_LIKE</th>";
				// echo 		"<th>$TABLE_COLUMN_FEEDBACK_DISLIKE</th>";
				// echo 		"<th>$TABLE_COLUMN_FEEDBACK_ERROR</th>";
				// echo 		"<th>$TABLE_COLUMN_FEEDBACK_TOO_HARD</th>";
				// echo 		"<th>$TABLE_COLUMN_FEEDBACK_TOO_EASY</th>";
				// echo 		"<th>$TABLE_COLUMN_FEEDBACK_NEED_COMMENT</th>";

				echo 		"<th>$TABLE_COLUMN_QUIZ_COMMENT_CNT</th>";

				echo "</tr>"; 
				echo "</thead>";
				echo "<tbody>";

				$__quiz_category_code = "";
				$__quiz_category = "";
				$__quiz_total_cnt = 0;
				$__quiz_cnt_qualified = 0;
				$__quiz_cnt_solved = 0;
				$__quiz_qualified_ratio = "";

				$__avg_time_lapse_qualified = "";
				$__avg_time_lapse_solved = "";

				$__sum_feedback_like = "";
				$__sum_feedback_dislike = "";
				$__sum_feedback_error = "";
				$__sum_feedback_too_hard = "";
				$__sum_feedback_too_easy = "";

				$__sum_comment_cnt = 0;


				if( strcmp($PROPS->QUIZ_CATEGORY, $param->QUIZ_CATEGORY_ALL_CATEGORY) == 0 ) {
					
					// 전체 카테고리 통계

					foreach($PROPS->QUIZ_CATEGORY_ARR as $category) {

						$category_stat = $quiz_category_stat->{$category};

						$__quiz_category_code = 
						$param->get_category_code(
							// $region=""
							$PROPS->QUIZ_REGION
							//, $language=""
							, $PROPS->QUIZ_LANGUAGE
							//, $category=""
							, $category
						);

						$__quiz_category = $category;
						$__quiz_total_cnt = intval($category_stat->quiz_cnt_by_category);
						$__quiz_cnt_qualified = intval($category_stat->quiz_cnt_qualified_by_category);
						$__quiz_cnt_solved = intval($category_stat->quiz_cnt_solved_by_category);
						$__quiz_qualified_ratio = $category_stat->quiz_ratio_correct_per_solved_by_category . "%";
						$__avg_time_lapse_qualified = $category_stat->quiz_avg_time_qualified_by_category;
						$__avg_time_lapse_solved = $category_stat->quiz_avg_time_solved_by_category;
						
						$__sum_feedback_like = $category_stat->user_feedback_like_cnt;
						$__sum_feedback_dislike = $category_stat->user_feedback_dislike_cnt;
						$__sum_feedback_error = $category_stat->user_feedback_error_cnt;
						$__sum_feedback_need_comment = $category_stat->user_feedback_need_comment_cnt;
						$__sum_feedback_too_hard = $category_stat->user_feedback_too_hard_cnt;
						$__sum_feedback_too_easy = $category_stat->user_feedback_too_easy_cnt;

						$feedback_btn_tags = 
						ServiceViewManager::get_feedback_field(
							// $PROPS=null
							$PROPS
							// $threshold_cnt_positive=5
							, $param->QUIZ_POSITIVE_FEEDBACK_WARNING_CNT
							// $threshold_cnt_negative=5
							, $param->QUIZ_NEGATIVE_FEEDBACK_WARNING_CNT
							// $feedback_like_cnt = 0
							, intval($__sum_feedback_like)
							// $feedback_dislike_cnt = 0
							, intval($__sum_feedback_dislike)
							// $feedback_error_cnt = 0
							, intval($__sum_feedback_error)
							// $feedback_need_comment_cnt = 0
							, intval($__sum_feedback_need_comment)
							// $feedback_too_easy_cnt = 0
							, intval($__sum_feedback_too_easy)
							// $feedback_too_hard_cnt = 0
							, intval($__sum_feedback_too_hard)
						);						

						
						$__sum_comment_cnt = $category_stat->user_comment_cnt;

						$msg = $__quiz_cnt_qualified . "/" . $__quiz_total_cnt . "($__quiz_qualified_ratio)";

						echo 	"<tr>";

						echo 		"<td>$__quiz_category_code<br/><span style=\"font-size:12px;color:$param->XOGAMES_NOT_SURE_GRAY;\">$__quiz_category</span></td>";
						echo 		"<td>$__quiz_total_cnt</td>";
						echo 		"<td>$__quiz_cnt_qualified/$__quiz_cnt_solved($__quiz_qualified_ratio)</td>";
						echo 		"<td>$__avg_time_lapse_qualified</td>";
						echo 		"<td>$__avg_time_lapse_solved</td>";

						echo 		"<td>$feedback_btn_tags</td>";

						echo 		"<td>$__sum_comment_cnt</td>";

						echo 	"</tr>";

					}					

				} else if(isset($category_stat)) {

					// 개별 카테고리 통계
					$__quiz_category = $PROPS->QUIZ_CATEGORY;
					$__quiz_category_code = 
					$param->get_category_code(
						// $region=""
						$PROPS->QUIZ_REGION
						//, $language=""
						, $PROPS->QUIZ_LANGUAGE
						//, $category=""
						, $__quiz_category
					);
					
					$__quiz_total_cnt = intval($category_stat->quiz_cnt_by_category);
					$__quiz_cnt_qualified = intval($category_stat->quiz_cnt_qualified_by_category);
					$__quiz_cnt_solved = intval($category_stat->quiz_cnt_solved_by_category);
					$__quiz_qualified_ratio = $category_stat->quiz_ratio_correct_per_solved_by_category . "%";
					$__avg_time_lapse_qualified = $category_stat->quiz_avg_time_qualified_by_category;
					$__avg_time_lapse_solved = $category_stat->quiz_avg_time_solved_by_category;

					$__sum_feedback_like = $category_stat->user_feedback_like_cnt;
					$__sum_feedback_dislike = $category_stat->user_feedback_dislike_cnt;
					$__sum_feedback_error = $category_stat->user_feedback_error_cnt;
					$__sum_feedback_too_hard = $category_stat->user_feedback_too_hard_cnt;
					$__sum_feedback_too_easy = $category_stat->user_feedback_too_easy_cnt;
					$__sum_feedback_need_comment = $category_stat->user_feedback_need_comment_cnt;

					$feedback_btn_tags = 
					ServiceViewManager::get_feedback_field(
						// $PROPS=null
						$PROPS
						// $threshold_cnt_positive=5
						, $param->QUIZ_POSITIVE_FEEDBACK_WARNING_CNT
						// $threshold_cnt_negative=5
						, $param->QUIZ_NEGATIVE_FEEDBACK_WARNING_CNT
						// $feedback_like_cnt = 0
						, intval($__sum_feedback_like)
						// $feedback_dislike_cnt = 0
						, intval($__sum_feedback_dislike)
						// $feedback_error_cnt = 0
						, intval($__sum_feedback_error)
						// $feedback_need_comment_cnt = 0
						, intval($__sum_feedback_need_comment)
						// $feedback_too_easy_cnt = 0
						, intval($__sum_feedback_too_easy)
						// $feedback_too_hard_cnt = 0
						, intval($__sum_feedback_too_hard)
					);

					$__sum_comment_cnt = $category_stat->user_comment_cnt;



					$msg = $__quiz_cnt_qualified . "/" . $__quiz_total_cnt . "($__quiz_qualified_ratio)";

					echo 	"<tr>";

					echo 		"<td>$__quiz_category_code<br/><span style=\"font-size:12px;color:$param->XOGAMES_NOT_SURE_GRAY;\">$__quiz_category</span></td>";
					echo 		"<td>$__quiz_total_cnt</td>";

					echo 		"<td>$__quiz_cnt_qualified/$__quiz_cnt_solved($__quiz_qualified_ratio)</td>";

					echo 		"<td>$__avg_time_lapse_qualified</td>";
					echo 		"<td>$__avg_time_lapse_solved</td>";

					echo 		"<td>$feedback_btn_tags</td>";

					echo 		"<td>$__sum_comment_cnt</td>";
				}


				echo 	"</tr>";

				echo "</tbody>";
			echo "</table>";
			// CATEGORY PROGRESS ENDS	


			// PAGENATION INIT
			echo $PROPS->PAGINATION->BOOTSTRAP_TAG;
			// PAGENATION DONE





	// CHECK & COMBO BOX SET - INIT
		echo "<ul class=\"list-group\" style=\"margin-bottom: 15px;\">";
			echo "<li class=\"list-group-item xo_control_box_bg_color\" style=\"padding-bottom:44px;\">";

			// ROW FIELD - CHECK BOX EACH - INIT
			$meta_data = new stdClass();
			$meta_data->quiz_id = "";
			$meta_data->key = "";

			$checkbox_obj = 
			CheckBoxManager::get_checkbox_chain(
				// $check_type=""
				CheckBoxManager::$TYPE_CHECK_ALL
				// $group_id="", 
				, "checkbox_quiz_list"
				// $check_box_id=""
				, ""
				// , $value=""
				, ""
				// , $meta_data_obj=null;
				, $meta_data
			);
			$HTML_TAG = $checkbox_obj->HTML_TAG;
			echo "$HTML_TAG";
			// ROW FIELD - CHECK BOX EACH - DONE	

			// BUTTON - NEW
			$BTN_NEW = $PROPS->BUTTON_SET->BTN_NEW;
			$HTML_TAG = $BTN_NEW->HTML_TAG;
			echo "$HTML_TAG";

			// BUTTON - TESTING
			$BTN_TESTING = $PROPS->BUTTON_SET->BTN_TESTING;
			$HTML_TAG = $BTN_TESTING->HTML_TAG;
			echo "$HTML_TAG";

			// BUTTON - QUALIFIED
			$BTN_QUALIFIED = $PROPS->BUTTON_SET->BTN_QUALIFIED;
			$HTML_TAG = $BTN_QUALIFIED->HTML_TAG;
			echo "$HTML_TAG";

			// BUTTON - DISABLED
			$BTN_DISABLED = $PROPS->BUTTON_SET->BTN_DISABLED;
			$HTML_TAG = $BTN_DISABLED->HTML_TAG;
			echo "$HTML_TAG";

			// BUTTON - DISABLED
			$BTN_DELETE = $PROPS->BUTTON_SET->BTN_DELETE;
			$HTML_TAG = $BTN_DELETE->HTML_TAG;
			echo "$HTML_TAG";

			echo "</li>";
		echo "</ul>";
	// CHECK & COMBO BOX SET - DONE			



			// QUIZ TABLE INIT
			$data_arr_to_export = array();

			echo "<table id=\"quiz_table\" class=\"table table-hover table-bordered\">";
				echo "<thead>";
				echo "<tr>";

				$data_row_to_export = array();

				$TABLE_COLUMN_QUIZ = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_QUIZ;
				$TABLE_COLUMN_MANAGER = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_MANAGER;
				$TABLE_COLUMN_AUTHOR = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_AUTHOR;
				$TABLE_COLUMN_QUIZ_STATUS = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_QUIZ_STATUS;
				$TABLE_COLUMN_LAST_UPDATE_USER = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_LAST_UPDATE_USER;
				$TABLE_COLUMN_TIME_UPDATED = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_TIME_UPDATED;
				$TABLE_COLUMN_INQUIRY = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_INQUIRY;
				array_push($data_row_to_export, $TABLE_COLUMN_INQUIRY);
				$TABLE_COLUMN_INQUIRY_UNITY_TAG = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_INQUIRY_UNITY_TAG;
				$TABLE_COLUMN_ANSWER = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_ANSWER;
				
				$TABLE_COLUMN_DESC = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_DESC;
				array_push($data_row_to_export, $TABLE_COLUMN_DESC);
				array_push($data_row_to_export, $TABLE_COLUMN_ANSWER);

				$TABLE_COLUMN_DESC_UNITY_TAG = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_DESC_UNITY_TAG;
				$TABLE_COLUMN_QUIZ_IMAGE = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_QUIZ_IMAGE;
				array_push($data_row_to_export, $TABLE_COLUMN_QUIZ_IMAGE);

				array_push($data_arr_to_export, $data_row_to_export);

				// HEADER - CHECKBOX - INIT
				$checkbox_obj = 
				CheckBoxManager::get_checkbox_chain(
					// $check_type=""
					CheckBoxManager::$TYPE_CHECK_ALL
					// $group_id="", 
					, "checkbox_quiz_list"
					// $check_box_id=""
					, ""
					// , $value=""
					, ""
					// , $meta_data_obj=null;
					, null
				);
				$html_tag = $checkbox_obj->HTML_TAG;
				echo 		"<th>";
				echo 			$html_tag;
				echo 		"</th>";	
				// HEADER - CHECKBOX - DONE

				echo 		"<th><small>$TABLE_COLUMN_CATEGORY</small><br/><small>$TABLE_COLUMN_QUIZ</small><br/><small>$TABLE_COLUMN_AUTHOR</small><br/></th>";
				echo 		"<th class=\"col-md-1\"><small>$TABLE_COLUMN_QUIZ_STATUS</small></th>";
				echo 		"<th><small>$TABLE_COLUMN_LAST_UPDATE_USER</small></th>";

				echo 		"<th>$TABLE_COLUMN_INQUIRY/$TABLE_COLUMN_DESC</th>";
				echo 		"<th>$TABLE_COLUMN_ANSWER</th>";
				echo 		"<th style=\"width:170px;\">$TABLE_COLUMN_QUIZ_IMAGE</th>";

				echo "</tr>"; 
				echo "</thead>";
				echo "<tbody>";

			// $QUIZ_STATUS_ARR - INIT	
			$QUIZ_STATUS_ARR = $PROPS->xogames_param->QUIZ_STATUS;
			$quiz_status_key_value_arr = array();
			for($idx = 0;$idx < count($QUIZ_STATUS_ARR); $idx++) {

				$quiz_status_obj = $QUIZ_STATUS_ARR[$idx];
				$quiz_status_key_value = new stdClass();
				$quiz_status_key_value->key = $quiz_status_obj->NAME;
				$quiz_status_key_value->value = $quiz_status_obj->CODE;

				if($idx == 1) { // Testing
					$quiz_status_key_value->color = ButtonManager::$COLOR_GREEN;
				} else if($idx == 2) { // Qualified
					$quiz_status_key_value->color = ButtonManager::$COLOR_BLUE;
				} else if($idx == 3) { // Qualified
					$quiz_status_key_value->color = ButtonManager::$COLOR_RED;
				}

				array_push($quiz_status_key_value_arr,$quiz_status_key_value);

			}
			// $QUIZ_STATUS_ARR - DONE

			// $QUIZ_ANSWER_ARR - INIT
			$QUIZ_ANSWER_ARR = $PROPS->xogames_param->QUIZ_ANSWER;
			$quiz_answer_key_value_arr = array();
			for($idx = 0;$idx < count($QUIZ_ANSWER_ARR); $idx++) {
				
				$quiz_answer_obj = $QUIZ_ANSWER_ARR[$idx];
				$quiz_answer_key_value = new stdClass();
				$quiz_answer_key_value->key = $quiz_answer_obj->NAME;
				$quiz_answer_key_value->value = $quiz_answer_obj->CODE;

				if($idx == 0) { // O
					$quiz_answer_key_value->color = ButtonManager::$COLOR_BLUE;
				} else if($idx == 1) { // X
					$quiz_answer_key_value->color = ButtonManager::$COLOR_RED;
				}

				array_push($quiz_answer_key_value_arr,$quiz_answer_key_value);

			}
			// $QUIZ_ANSWER_ARR - DONE

			foreach($quiz_list_for_export as $quiz) {

				$data_row_to_export = array();

				$__inquiry = $quiz->__inquiry;
				array_push($data_row_to_export, "$__inquiry");
				$__answer = $quiz->__answer;

				$__desc = $quiz->__desc;
				$__desc_unity = $quiz->__desc_unity;
				$__desc_length = intval($quiz->__desc_length);

				array_push($data_row_to_export, "$__desc");
				array_push($data_row_to_export, "$__answer");

				$__img_link = $quiz->__img_link;
				array_push($data_row_to_export, "$__img_link");
				$__img_link_extra = $quiz->__img_link_extra;
				array_push($data_row_to_export, "$__img_link_extra");

				array_push($data_arr_to_export, $data_row_to_export);
			}
			$PROPS->data_arr_to_export = $data_arr_to_export;		

			for ($i=0; $i < count($quiz_list); $i++) { 

				$quiz = $quiz_list[$i];

				$data_row_to_export = array();

				$__quiz_category = $quiz->__category;
				$__quiz_category_code = $param->get_category_code($PROPS->QUIZ_REGION, $PROPS->QUIZ_LANGUAGE, $__quiz_category);
				$__quiz_id = $quiz->__id;

				$__quiz_status = $quiz->__status;
				$__quiz_status_code = $param->get_quiz_status_code($__quiz_status);
				
				$__quiz_status_color = $param->get_quiz_status_color($__quiz_status);

				$__author_name = $quiz->__author_name;

				$__time_created = $quiz->__time_created;
				$__time_update = $quiz->__time_update;
				$__is_updated = intval($quiz->__is_updated);
				$__last_updated_user_name = $quiz->__last_updated_user_name;

				$__inquiry = $quiz->__inquiry;
				$__inquiry_unity = $quiz->__inquiry_unity;
				$__inquiry_length = intval($quiz->__inquiry_length);
				$__inquiry_length_no_unity = intval($quiz->__inquiry_length_no_unity);

				$__answer = $quiz->__answer;

				$__desc = $quiz->__desc;
				$__desc_unity = $quiz->__desc_unity;
				$__desc_length = intval($quiz->__desc_length);
				$__desc_length_no_unity = intval($quiz->__desc_length_no_unity);

				$__ratio_user_correct = $quiz->__ratio_user_correct;
				$ratio = intval($quiz->__ratio_user_correct_num);

				$__solved_user_cnt = intval($quiz->__solved_user_cnt);
				$__img_link = $quiz->__img_link;
				$__img_link_extra = $quiz->__img_link_extra;
				$__thumbnail = $quiz->__thumbnail;
				$__thumbnail_extra = $quiz->__thumbnail_extra;
		

				// FIRST ROW - INIT
				echo 	"<tr id=\"content_head\">";

				// ROW FIELD - CHECK BOX EACH - INIT
				$meta_data = new stdClass();
				$meta_data->quiz_id = $__quiz_id;
				$meta_data->key = $param->QUIZ_STATUS;

				$checkbox_obj = 
				CheckBoxManager::get_checkbox_chain(
					// $check_type=""
					CheckBoxManager::$TYPE_CHECK_EACH
					// $group_id="", 
					, "checkbox_quiz_list"
					// $check_box_id=""
					, $__quiz_id
					// , $value=""
					, "$__quiz_id"
					// , $meta_data_obj=null;
					, $meta_data
				);
				$html_tag = $checkbox_obj->HTML_TAG;
				echo 		"<td rowspan=\"2\">";
				echo 			$html_tag;
				echo 		"</td>";
				// ROW FIELD - CHECK BOX EACH - DONE				
				
				echo 		"<td rowspan=\"2\" id=\"__quiz_id\" __quiz_id=\"$__quiz_id\">";
				echo 			"<span __category=\"$__quiz_category\"><small>$__quiz_category_code</small></span><br/>";
				echo 			"<span __quiz_id=\"$__quiz_id\"><small>$__quiz_id</small></span><br/>";
				echo 			"<span __author_name=\"$__author_name\"><small>$__author_name</small></span>";
				echo 		"</td>";

				// BTN CHAIN - QUIZ STATUS - INIT
				$meta_data = new stdClass();
				$meta_data->quiz_id = $__quiz_id;
				$meta_data->key = $param->QUIZ_STATUS;
				$view_input_obj = 
				ButtonManager::get_button_chain(
					// $btn_title_arr=null
					$quiz_status_key_value_arr
					// , $btn_size=""
					, ButtonManager::$SIZE_BIG
					// , $selected_key=""
					, $__quiz_status_code
					// , $btn_id=""
					, $__quiz_id
					// , $meta_data_obj=null
					, $meta_data
				);
				$html_tag = $view_input_obj->HTML_TAG;
				echo 		"<td rowspan=\"2\" id=\"__status\">";
				// echo 		"<td id=\"__status\">";
				echo 			"$html_tag";
				echo 		"</td>";
				// BTN CHAIN - QUIZ STATUS - DONE

				echo 		"<td rowspan=\"2\" id=\"__time_update\">";
				echo 			"<small>";
				echo 			"<span id=\"__last_updated_user_name\"><small>$__last_updated_user_name</small></span><br/>";
				echo 			"<span id=\"__time_update\"><small>$__time_update</small></span>";

				// INDICATE QUIZ UPDATE - INIT
				$updated_notice_display = "display:none;";
				if($__is_updated === 1) {
					$updated_notice_display = "";
				}
				echo "<br/><button type=\"button\" id=\"quiz_updated\" class=\"btn btn-warning btn-xs\" style=\"$updated_notice_display\">Updated!</button>";
				// INDICATE QUIZ UPDATE - DONE

				echo 			"</small>";
				echo 		"</td>";

				// VIEW INPUT - QUIZ INQUIRY - INIT
				$meta_data = new stdClass();
				$meta_data->quiz_id = $__quiz_id;
				$meta_data->key = $param->QUIZ_INQUIRY;
				$view_input_obj = 
				InputManager::get_view_input_max_char(
					// $title=""
					"view_input_inquiry"
					// $max_char=-1
					, $param->QUIZ_INQUIRY_CHAR_LIMIT
					// $text_on_view=""
					, $__inquiry_unity
					// $text_on_input=""
					, $__inquiry
					// , $text_length=-1
					, $__inquiry_length_no_unity
					// , $header_text=""
					, $TABLE_COLUMN_INQUIRY
					// , $header_color=""
					, InputManager::$COLOR_BLUE
					// , $meta_data
					, $meta_data
				);
				$html_tag = $view_input_obj->HTML_TAG;
				echo 		"<td id=\"__inquiry\">";
				echo 			"$html_tag";
				echo 		"</td>";
				// VIEW INPUT - QUIZ INQUIRY - DONE				

				// ROW FILED - QUIZ_ANSWER - INIT
				$meta_data = new stdClass();
				$meta_data->quiz_id = $__quiz_id;
				$meta_data->key = $param->QUIZ_ANSWER;
				$view_input_obj = 
				ButtonManager::get_button_chain(
					// $btn_title_arr=null
					$quiz_answer_key_value_arr
					// , $btn_size=""
					, ButtonManager::$SIZE_BIG
					// , $selected_key=""
					, $__answer
					// , $btn_id=""
					, $__quiz_id
					// , $meta_data_obj=null
					, $meta_data
				);
				$html_tag = $view_input_obj->HTML_TAG;
				echo "<td rowspan=\"2\" id=\"__answer\">";
				// echo "<td id=\"__answer\">";
				echo 	$html_tag;
				echo "</td>";
				// ROW FILED - QUIZ_ANSWER - DONE				


				// ROW FIELD - QUIZ IMAGE - INIT
				// IMAGE LINK
				$meta_data = new stdClass();
				$meta_data->quiz = $quiz;
				$meta_data->key = $param->QUIZ_IMG_LINK;
				$meta_data->image_desc = $quiz->__img_link_src;
				$meta_data->float_left = true;

				$image_input_obj = 
				InputManager::get_image_input(
					// $img_url_on_input=""
					$quiz->__img_link
					// , $img_url_on_view=""
					, $quiz->__loadable_img_link
					// , $img_width=200
					, 70
					// , $on_error_img_url=""
					, $PROPS->ON_ERROR_IMG_URL
					// , $meta_data_obj=null
					, $meta_data
				);

				// IMAGE LINK EXTRA
				$meta_data = new stdClass();
				$meta_data->quiz = $quiz;
				$meta_data->key = $param->QUIZ_IMG_LINK_EXTRA;
				$meta_data->image_desc = $quiz->__img_link_extra_src;
				$meta_data->float_left = true;
				$meta_data->margin_left = 10;
				
				$image_input_extra_obj = 
				InputManager::get_image_input(
					// $img_url_on_input=""
					$quiz->__img_link_extra
					// , $img_url_on_view=""
					, $quiz->__loadable_img_link_extra
					// , $img_width=200
					, 70
					// , $on_error_img_url=""
					, $PROPS->ON_ERROR_IMG_URL
					// , $meta_data_obj=null
					, $meta_data
				);

				$html_tag = $image_input_obj->HTML_TAG . $image_input_extra_obj->HTML_TAG;
				echo "<td rowspan=\"2\" id=\"__img_link\">";
				echo 	$html_tag;
				echo "</td>";
				// ROW FIELD - QUIZ IMAGE - DONE


				echo 	"</tr>";
				// FIRST ROW - DONE

				// SECOND ROW - INIT
				echo 	"<tr id=\"content_tail\">";

				$meta_data = new stdClass();
				$meta_data->quiz_id = $__quiz_id;
				$meta_data->key = $param->QUIZ_DESC;
				$view_input_obj = 
				InputManager::get_view_input_max_char(
					// $title=""
					"view_input_desc"
					// $max_char=-1
					, $param->QUIZ_DESC_CHAR_LIMIT
					// $text_on_view=""
					, $__desc_unity
					// $text_on_input=""
					, $__desc
					// , $text_length=-1
					, $__desc_length_no_unity
					// , $header_text=""
					, $TABLE_COLUMN_DESC
					// , $header_color=""
					, InputManager::$COLOR_GREEN
					// , $meta_data
					, $meta_data
				);
				$html_tag = $view_input_obj->HTML_TAG;

				echo 		"<td id=\"__desc\">";
				echo 			"$html_tag";
				echo 		"</td>";
				echo 	"</tr>";
				// SECOND ROW - DONE

			} // end for each
			

				echo "</tbody>";
			echo "</table>";
			// QUIZ TABLE DONE






			// PAGENATION INIT
			echo $PROPS->PAGINATION->BOOTSTRAP_TAG;
			// PAGENATION DONE

				echo "</div>";
			echo "</div>";			

		?>

















		
	</div><!--/.main-->

<script>

// php to javascript sample
var PROPS = <?php echo json_encode($PROPS);?>;

var quiz_status_arr = PROPS.QUIZ_STATUS_ARR;
var quiz_status_code_set = PROPS.QUIZ_STATUS_CODE_SET;
var quiz_status_color_set = PROPS.QUIZ_STATUS_COLOR_SET;

var quiz_table_jq = $("table#quiz_table");


// EVENT HEADS-UP 
// xogames_headsup
var xogames_headsup_jq = $("div#xogames_headsup");
abc_headsup.set(xogames_headsup_jq);
if(PROPS.GD_LIBLARY == null || !PROPS.GD_LIBLARY) {
	abc_headsup._show_warning("Warning!","GD Library is not valid!");
}


// set event
// on change select event - region
var COMBO_BOX_ID = PROPS.COMBO_BOX_SET.COMBO_BOX_REGION.COMBO_BOX_ID;
var select_region_jq = $("select#" + COMBO_BOX_ID);
select_region_jq.change(function(){
	var selected_value = $(this).val();

	_link.refresh_post(
		_param
		.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
		.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)
		.get(PROPS.PARAM_SET.QUIZ_REGION, selected_value)
		.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, PROPS.QUIZ_LANGUAGE)
		.get(PROPS.PARAM_SET.QUIZ_CATEGORY, PROPS.QUIZ_CATEGORY)
		.get(PROPS.PARAM_SET.QUIZ_STATUS, PROPS.QUIZ_STATUS)
		.get(PROPS.PARAM_SET.ROW_CNT, PROPS.ROW_CNT)
		.get(PROPS.PARAM_SET.TARGET_AUTHOR_NAME, "")
	);

});
// on change select event - langauge
var COMBO_BOX_ID = PROPS.COMBO_BOX_SET.COMBO_BOX_LANGUAGE.COMBO_BOX_ID;
var select_language_jq = $("select#" + COMBO_BOX_ID);
select_language_jq.change(function(){
	var selected_value = $(this).val();

	_link.refresh_post(
		_param
		.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
		.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)
		.get(PROPS.PARAM_SET.QUIZ_REGION, PROPS.QUIZ_REGION)
		.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, selected_value)
		.get(PROPS.PARAM_SET.QUIZ_CATEGORY, PROPS.QUIZ_CATEGORY)
		.get(PROPS.PARAM_SET.QUIZ_STATUS, PROPS.QUIZ_STATUS)
		.get(PROPS.PARAM_SET.ROW_CNT, PROPS.ROW_CNT)
		.get(PROPS.PARAM_SET.TARGET_AUTHOR_NAME, "")
	);

});
// on change select event - category
var COMBO_BOX_ID = PROPS.COMBO_BOX_SET.COMBO_BOX_CATEGORY_ALL.COMBO_BOX_ID;
var select_category_all_jq = $("select#" + COMBO_BOX_ID);
select_category_all_jq.change(function(){

	var selected_value = $(this).val();

	_link.refresh_post(
		_param
		.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
		.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)
		.get(PROPS.PARAM_SET.QUIZ_REGION, PROPS.QUIZ_REGION)
		.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, PROPS.QUIZ_LANGUAGE)
		.get(PROPS.PARAM_SET.QUIZ_CATEGORY, selected_value)
		.get(PROPS.PARAM_SET.QUIZ_STATUS, PROPS.QUIZ_STATUS)
		.get(PROPS.PARAM_SET.ROW_CNT, PROPS.ROW_CNT)
		.get(PROPS.PARAM_SET.TARGET_AUTHOR_NAME, "")
	);

});
// on change select event - quiz status
var COMBO_BOX_ID = PROPS.COMBO_BOX_SET.COMBO_BOX_QUIZ_STATUS.COMBO_BOX_ID;
var select_quiz_status_jq = $("select#" + COMBO_BOX_ID);
select_quiz_status_jq.change(function(){

	var selected_value = $(this).val();

	_link.refresh_post(
		_param
		.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
		.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)
		.get(PROPS.PARAM_SET.QUIZ_REGION, PROPS.QUIZ_REGION)
		.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, PROPS.QUIZ_LANGUAGE)
		.get(PROPS.PARAM_SET.QUIZ_CATEGORY, PROPS.QUIZ_CATEGORY)
		.get(PROPS.PARAM_SET.QUIZ_STATUS, selected_value)
		.get(PROPS.PARAM_SET.ROW_CNT, PROPS.ROW_CNT)
		.get(PROPS.PARAM_SET.TARGET_AUTHOR_NAME, "")
	);

});
// on change select event - row cnt
var COMBO_BOX_ID = PROPS.COMBO_BOX_SET.COMBO_BOX_ROW_CNT.COMBO_BOX_ID;
var select_row_cnt_jq = $("select#" + COMBO_BOX_ID);
select_row_cnt_jq.change(function(){
	var selected_value = $(this).val();

	_link.refresh_post(
		_param
		.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
		.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)
		.get(PROPS.PARAM_SET.QUIZ_REGION, PROPS.QUIZ_REGION)
		.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, PROPS.QUIZ_LANGUAGE)
		.get(PROPS.PARAM_SET.QUIZ_CATEGORY, PROPS.QUIZ_CATEGORY)
		.get(PROPS.PARAM_SET.QUIZ_STATUS, PROPS.QUIZ_STATUS)
		.get(PROPS.PARAM_SET.ROW_CNT, selected_value)
		.get(PROPS.PARAM_SET.TARGET_AUTHOR_NAME, "")
	);

});










var btn_show_jq = $("button#" + PROPS.BUTTON_SET.BTN_SHOW.ID);
btn_show_jq.click(function(e){

	_link.refresh_post(
		_param
		.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
		.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)
		.get(PROPS.PARAM_SET.QUIZ_REGION, PROPS.QUIZ_REGION)
		.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, PROPS.QUIZ_LANGUAGE)
		.get(PROPS.PARAM_SET.QUIZ_CATEGORY, PROPS.QUIZ_CATEGORY)
		.get(PROPS.PARAM_SET.QUIZ_STATUS, PROPS.QUIZ_STATUS)
		.get(PROPS.PARAM_SET.ROW_CNT, PROPS.ROW_CNT)
		.get(PROPS.PARAM_SET.TARGET_AUTHOR_NAME, PROPS.TARGET_AUTHOR_NAME)
	);

});
var btn_export_jq = $("button#" + PROPS.BUTTON_SET.BTN_EXPORT.ID);
btn_export_jq.click(function(e) {
	var latest_quiz_list_csv = quiz_factory.convert(PROPS.data_arr_to_export);
	var file_name = PROPS.QUIZ_REGION + "_" + PROPS.QUIZ_CATEGORY
	quiz_factory.download(latest_quiz_list_csv, file_name);
});






// search btn
//padding-left: 10px;

var btn_search_jq = $("button#btn_search");
btn_search_jq.click(function(e){

	var _self_jq = $(this);
	var keyword = _self_jq.parent().parent().find("input").val();

	if(_v.is_not_valid_str(keyword)) {
		return;
	}

	_link.refresh_post(
		_param
		.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
		.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)
		.get(PROPS.PARAM_SET.QUIZ_REGION, PROPS.QUIZ_REGION)
		.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, PROPS.QUIZ_LANGUAGE)
		.get(PROPS.PARAM_SET.QUIZ_CATEGORY, PROPS.QUIZ_CATEGORY)
		.get(PROPS.PARAM_SET.QUIZ_STATUS, PROPS.QUIZ_STATUS)
		.get(PROPS.PARAM_SET.ROW_CNT, PROPS.ROW_CNT)
		.get(PROPS.PARAM_SET.TARGET_AUTHOR_NAME, PROPS.TARGET_AUTHOR_NAME)
		.get(PROPS.PARAM_SET.SEARCH_KEYWORD, keyword)
	);

});

$( document ).keyup(function( event ) {

	if(event.which === 13) {
		// ENTER 

		var search_keyword_input_jq = $("div#quiz_search input");
		var search_keyword_str = search_keyword_input_jq.val();

		if(_v.is_not_valid_str(search_keyword_str)) {
			return;
		}

		_link.refresh_post(
			_param
			.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
			.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)
			.get(PROPS.PARAM_SET.QUIZ_REGION, PROPS.QUIZ_REGION)
			.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, PROPS.QUIZ_LANGUAGE)
			.get(PROPS.PARAM_SET.QUIZ_CATEGORY, PROPS.QUIZ_CATEGORY)
			.get(PROPS.PARAM_SET.QUIZ_STATUS, PROPS.QUIZ_STATUS)
			.get(PROPS.PARAM_SET.ROW_CNT, PROPS.ROW_CNT)
			.get(PROPS.PARAM_SET.TARGET_AUTHOR_NAME, PROPS.TARGET_AUTHOR_NAME)
			.get(PROPS.PARAM_SET.SEARCH_KEYWORD, search_keyword_str)
		);
	}
});






// MODIFY.

// SET EVENT - UPDATE INQUIRY - INIT
var __inquiry_jq_arr = quiz_table_jq.find("tr#content_head td#__inquiry");
abc_view_input.set(
	__inquiry_jq_arr
	, this
	, function(meta_obj){

		var quiz_id = parseInt(meta_obj.quiz_id);
		var key = meta_obj.key;
		var value = meta_obj.value;
		var view_text_jq = meta_obj.view_text_jq;
		var container_jq = meta_obj.container_jq;

		if(_v.is_not_valid_str(key)) {
			return;
		}
		if(PROPS.PARAM_SET.QUIZ_INQUIRY !== key) {
			return;
		}
		if(_v.is_not_valid_str(value)) {
			return;
		}
		if(view_text_jq == null) {
			return;
		}

		// SET UNITY TAG
		view_text_jq.html(quiz_manager.parse_unity_tag(value, PROPS.xogames_param.UNITY_TAG_COLOR));

		var request_param_obj = 
		_param
		.get(PROPS.PARAM_SET.EVENT_TYPE, PROPS.PARAM_SET.EVENT_TYPE_UPDATE_QUIZ_INQUIRY)
		.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
		.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)
		.get(PROPS.PARAM_SET.QUIZ_REGION, PROPS.QUIZ_REGION)
		.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, PROPS.QUIZ_LANGUAGE)
		.get(PROPS.PARAM_SET.QUIZ_CATEGORY, PROPS.QUIZ_CATEGORY)
		.get(PROPS.PARAM_SET.QUIZ_ID, quiz_id)
		.get(PROPS.PARAM_SET.QUIZ_INQUIRY, value)
		;

		// ajax - quiz inquiry update
		_ajax.post(
			// _url
			_link.get_link(_link.API_UPDATE_USER_QUIZ)
			// _param_obj
			,request_param_obj
			// _delegate_after_job_done
			,_obj.get_delegate(
				// delegate_func
				function(data){
					
					if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
						console.log(">>> activate_modal / data :: ",data);
					}

					if(!data.SUCCESS) {
						console.log("Error! / #775");
					}

					var parent_jq = container_jq.parent();
					var btn_quiz_updated_jq = container_jq.parent().find("button#quiz_updated");
					if(!btn_quiz_updated_jq.is(":visible")) {
						btn_quiz_updated_jq.show();
					}

				},
				// delegate_scope
				this
			)
		); // ajax done.

	}
	// callback_before_show
	, function(user_input) {
		return quiz_manager.parse_html_inline_safe(user_input);
	}
	// callback_str_cnt
	, function(user_input_str){

		if(_v.is_not_valid_str(user_input_str)) {
			return -1;
		}

		var unity_tag_cnt = quiz_manager.cnt_unity_tag(user_input_str);
		if( (0 < user_input_str.length) && 
			(0 < unity_tag_cnt) && 
			(unity_tag_cnt <= user_input_str.length)) {
			return user_input_str.length - unity_tag_cnt;
		}

		return -1;

	}	
);
// SET EVENT - UPDATE INQUIRY - DONE









// SET EVENT - UPDATE QUIZ DESC - INIT
var __desc_jq_arr = quiz_table_jq.find("tr#content_tail td#__desc");
abc_view_input.set(
	__desc_jq_arr
	, this
	, function(meta_obj){

		var quiz_id = parseInt(meta_obj.quiz_id);
		var key = meta_obj.key;
		var value = meta_obj.value;
		var view_text_jq = meta_obj.view_text_jq;
		var container_jq = meta_obj.container_jq;

		if(_v.is_not_valid_str(key)) {
			return;
		}
		if(PROPS.PARAM_SET.QUIZ_DESC !== key) {
			return;
		}
		if(view_text_jq == null) {
			return;
		}

		// SET UNITY TAG
		view_text_jq.html(quiz_manager.parse_unity_tag(value, PROPS.xogames_param.UNITY_TAG_COLOR));

		var request_param_obj = 
		_param
		.get(PROPS.PARAM_SET.EVENT_TYPE, PROPS.PARAM_SET.EVENT_TYPE_UPDATE_QUIZ_DESC)
		.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
		.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)
		.get(PROPS.PARAM_SET.QUIZ_REGION, PROPS.QUIZ_REGION)
		.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, PROPS.QUIZ_LANGUAGE)
		.get(PROPS.PARAM_SET.QUIZ_CATEGORY, PROPS.QUIZ_CATEGORY)
		.get(PROPS.PARAM_SET.QUIZ_ID, quiz_id)
		.get(PROPS.PARAM_SET.QUIZ_DESC, value)
		;

		// ajax - quiz inquiry update
		_ajax.post(
			// _url
			_link.get_link(_link.API_UPDATE_USER_QUIZ)
			// _param_obj
			,request_param_obj
			// _delegate_after_job_done
			,_obj.get_delegate(
				// delegate_func
				function(data){
					
					if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
						console.log(">>> activate_modal / data :: ",data);
					}

					if(!data.SUCCESS) {
						console.log("Error! / #858");
					}

					var btn_quiz_updated_jq = container_jq.parent().prev().find("button#quiz_updated");
					if(!btn_quiz_updated_jq.is(":visible")) {
						btn_quiz_updated_jq.show();
					}					

				},
				// delegate_scope
				this
			)
		); // ajax done.

	}
	// callback_before_show
	, function(user_input) {
		return quiz_manager.parse_html_inline_safe(user_input);
	}
	// callback_str_cnt
	, function(user_input_str){

		if(_v.is_not_valid_str(user_input_str)) {
			return -1;
		}

		var unity_tag_cnt = quiz_manager.cnt_unity_tag(user_input_str);
		if( (0 < user_input_str.length) && 
			(0 < unity_tag_cnt) && 
			(unity_tag_cnt <= user_input_str.length)) {
			return user_input_str.length - unity_tag_cnt;
		}

		return -1;

	}
);
// SET EVENT - UPDATE QUIZ DESC - DONE
var image_input_jq_arr = quiz_table_jq.find("div#AIRBORNE_IMAGE_INPUT");
abc_image_input.set(
	// target_jq_arr
	image_input_jq_arr
	// scope
	, this
	// callback_on_save_img_url 
	, callback_manager.image_input_view_on_save_img_url
	// callback_on_save_img_desc
	, callback_manager.image_input_view_on_save_img_desc
	// callback_when_event_received
	, callback_manager.image_input_view_on_click_quiz
);
// SET EVENT - UPDATE QUIZ IMAGE LINK - DONE





// SET EVENT - UPDATE QUIZ STATUS - INIT
var btn_chain_status_jq_arr = quiz_table_jq.find("td#__answer div#BUTTON_CHAIN_GROUP");	
abc_button_chain.set(
	// target_jq_arr
	btn_chain_status_jq_arr
	// scope
	, this
	// callback_when_done
	, function(data){

		var quiz_id = parseInt(data.quiz_id);
		var key = data.key;
		var value = data.value;
		var target_jq = data.target_jq;

		if(_v.is_not_valid_str(key)) {
			return;
		}
		if(PROPS.PARAM_SET.QUIZ_ANSWER !== key) {
			return;
		}
		if(_v.is_not_valid_str(value)) {
			return;
		}

		var request_param_obj = 
		_param
		.get(PROPS.PARAM_SET.EVENT_TYPE, PROPS.PARAM_SET.EVENT_TYPE_UPDATE_QUIZ_ANSWER)
		.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
		.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)
		.get(PROPS.PARAM_SET.QUIZ_REGION, PROPS.QUIZ_REGION)
		.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, PROPS.QUIZ_LANGUAGE)
		.get(PROPS.PARAM_SET.QUIZ_CATEGORY, PROPS.QUIZ_CATEGORY)
		.get(PROPS.PARAM_SET.QUIZ_ID, quiz_id)
		.get(PROPS.PARAM_SET.QUIZ_ANSWER, value)
		.get(PROPS.PARAM_SET.PAGE_NUM, PROPS.PAGE_NUM)
		.get(PROPS.PARAM_SET.ROW_CNT, PROPS.ROW_CNT)
		;

		// ajax - quiz inquiry update
		_ajax.post(
			// _url
			_link.get_link(_link.API_UPDATE_USER_QUIZ)
			// _param_obj
			, request_param_obj
			// _delegate_after_job_done
			, _obj.get_delegate(
				// delegate_func
				function(data){
					
					if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
						console.log("EVENT_TYPE_UPDATE_QUIZ_ANSWER / data :: ",data);
					}

					if(!data.SUCCESS) {
						console.log("Error! / #1048");
					}

					// show "Updated!"
					var row_jq = target_jq.parent().parent().parent();
					var btn_quiz_updated_jq = row_jq.find("button#quiz_updated");
					if(!btn_quiz_updated_jq.is(":visible")) {
						btn_quiz_updated_jq.show();
					}

				},
				// delegate_scope
				this
			)
		); // ajax done.
	}
	// callback_when_event_received
	, function() {

		console.log("callback_when_event_received");

		// stop further process
		return true;
	}	
);
// SET EVENT - UPDATE QUIZ ANSWER - DONE







// SET EVENT - UPDATE QUIZ STATUS - INIT
var btn_chain_status_jq_arr = quiz_table_jq.find("td#__status div#BUTTON_CHAIN_GROUP");	
abc_button_chain.set(
	// target_jq_arr
	btn_chain_status_jq_arr
	// scope
	, this
	// callback_when_done
	, function(data){

		var quiz_id = parseInt(data.quiz_id);
		var key = data.key;
		var value = data.value;

		if(_v.is_not_valid_str(key)) {
			return;
		}
		if(PROPS.PARAM_SET.QUIZ_STATUS !== key) {
			return;
		}
		if(_v.is_not_valid_str(value)) {
			return;
		}

		var request_param_obj = 
		_param
		.get(PROPS.PARAM_SET.EVENT_TYPE, PROPS.PARAM_SET.EVENT_TYPE_UPDATE_QUIZ_STATUS)
		.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
		.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)
		.get(PROPS.PARAM_SET.QUIZ_REGION, PROPS.QUIZ_REGION)
		.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, PROPS.QUIZ_LANGUAGE)
		.get(PROPS.PARAM_SET.QUIZ_CATEGORY, PROPS.QUIZ_CATEGORY)
		.get(PROPS.PARAM_SET.QUIZ_ID, quiz_id)
		.get(PROPS.PARAM_SET.QUIZ_STATUS, value)
		.get(PROPS.PARAM_SET.PAGE_NUM, PROPS.PAGE_NUM)
		.get(PROPS.PARAM_SET.ROW_CNT, PROPS.ROW_CNT)
		;

		// ajax - quiz inquiry update
		_ajax.post(
			// _url
			_link.get_link(_link.API_UPDATE_USER_QUIZ)
			// _param_obj
			, request_param_obj
			// _delegate_after_job_done
			, _obj.get_delegate(
				// delegate_func
				function(data){
					
					if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
						console.log("EVENT_TYPE_UPDATE_QUIZ_STATUS / data :: ",data);
					}

					if(!data.SUCCESS) {
						console.log("Error! / #858");
					}

				},
				// delegate_scope
				this
			)
		); // ajax done.

	}
	// callback_when_event_received
	, function() {

		console.log("callback_when_event_received");

		// stop further process
		return true;
	}	
);
// SET EVENT - UPDATE QUIZ STATUS - DONE







// SET EVENT - DELETE QUIZ LIST - INIT
var checkbox_chain_jq_arr = $("div#checkbox_quiz_list");
abc_checkbox_input.set(checkbox_chain_jq_arr, function(meta_data){

	var checked_values = abc_checkbox_input.get_checked_values();
	if(_v.is_not_valid_array(checked_values)) {
		return;
	}

	console.log("meta_data :: ",meta_data);

}, this);
abc_checkbox_input.get_checked_values();
// SET EVENT - DELETE QUIZ LIST - DONE



// SET EVENT - UPDATE QUIZ STATUS - INIT
var update_quiz_status = function(new_status, quiz_id_arr) {
	// 선택된 퀴즈 리스트내의 퀴즈 상태를 바꿉니다.

	if(_v.is_not_valid_str(new_status)) {
		return;
	}
	if(_v.is_not_valid_array(quiz_id_arr)) {
		return;
	}

	console.log("update_quiz_status / new_status ::: ",new_status);

	var quiz_id_list_str = JSON.stringify(quiz_id_arr);

	var request_param_obj = 
	_param
	.get(PROPS.PARAM_SET.EVENT_TYPE, PROPS.PARAM_SET.EVENT_TYPE_UPDATE_MULTIPLE_QUIZ_STATUS)
	.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
	.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)
	.get(PROPS.PARAM_SET.QUIZ_REGION, PROPS.QUIZ_REGION)
	.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, PROPS.QUIZ_LANGUAGE)
	.get(PROPS.PARAM_SET.QUIZ_CATEGORY, PROPS.QUIZ_CATEGORY)
	.get(PROPS.PARAM_SET.QUIZ_ID_IN_ARR_STR, quiz_id_list_str)
	.get(PROPS.PARAM_SET.QUIZ_STATUS, new_status)
	;

	// ajax - quiz inquiry update
	_ajax.post(
		// _url
		_link.get_link(_link.API_UPDATE_USER_QUIZ)
		// _param_obj
		, request_param_obj
		// _delegate_after_job_done
		, _obj.get_delegate(
			// delegate_func
			function(data){
				
				if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
					console.log("ACTION_TYPE_UPDATE_QUIZ_STATUS_MULTIPLE / data :: ",data);
				}

				if(!data.SUCCESS) {
					console.log("Error! / #858");
					return;
				}

				_link.refresh_post(
					_param
					.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
					.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)
					.get(PROPS.PARAM_SET.QUIZ_REGION, PROPS.QUIZ_REGION)
					.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, PROPS.QUIZ_LANGUAGE)
					.get(PROPS.PARAM_SET.QUIZ_CATEGORY, PROPS.QUIZ_CATEGORY)
					.get(PROPS.PARAM_SET.SEARCH_KEYWORD, PROPS.SEARCH_KEYWORD)
					.get(PROPS.PARAM_SET.QUIZ_STATUS, PROPS.QUIZ_STATUS)
					.get(PROPS.PARAM_SET.PAGE_NUM, PROPS.PAGE_NUM)
					.get(PROPS.PARAM_SET.ROW_CNT, PROPS.ROW_CNT)
				);

			},
			// delegate_scope
			this
		)
	); // ajax done.

}
var btn_new_jq = $("button#" + PROPS.BUTTON_SET.BTN_NEW.ID);
btn_new_jq.click(function(e) {
	var checked_values = abc_checkbox_input.get_checked_values();
	var status_new = PROPS.xogames_param.QUIZ_STATUS[0].CODE;

	update_quiz_status(status_new, checked_values);
});
var btn_testing_jq = $("button#" + PROPS.BUTTON_SET.BTN_TESTING.ID);
btn_testing_jq.click(function(e) {
	var checked_values = abc_checkbox_input.get_checked_values();
	var status_new = PROPS.xogames_param.QUIZ_STATUS[1].CODE;

	update_quiz_status(status_new, checked_values);
});
var btn_qualified_jq = $("button#" + PROPS.BUTTON_SET.BTN_QUALIFIED.ID);
btn_qualified_jq.click(function(e) {
	var checked_values = abc_checkbox_input.get_checked_values();
	var status_new = PROPS.xogames_param.QUIZ_STATUS[2].CODE;

	update_quiz_status(status_new, checked_values);
});
var btn_disabled_jq = $("button#" + PROPS.BUTTON_SET.BTN_DISABLED.ID);
btn_disabled_jq.click(function(e) {
	var checked_values = abc_checkbox_input.get_checked_values();
	var status_new = PROPS.xogames_param.QUIZ_STATUS[3].CODE;

	update_quiz_status(status_new, checked_values);
});
var btn_delete_jq = $("button#" + PROPS.BUTTON_SET.BTN_DELETE.ID);
btn_delete_jq.click(function(e) {

	// 복수개의 퀴즈 삭제
	var checked_values = abc_checkbox_input.get_checked_values();
	var quiz_json_arr_str = JSON.stringify(checked_values);

	var request_param_obj = 
	_param
	.get(PROPS.PARAM_SET.EVENT_TYPE, PROPS.PARAM_SET.EVENT_TYPE_DELETE_QUIZ_LIST)
	.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
	.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)
	.get(PROPS.PARAM_SET.QUIZ_REGION, PROPS.QUIZ_REGION)
	.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, PROPS.QUIZ_LANGUAGE)
	.get(PROPS.PARAM_SET.QUIZ_CATEGORY, PROPS.QUIZ_CATEGORY)
	.get(PROPS.PARAM_SET.QUIZ_JSON_ARR_STR, quiz_json_arr_str)
	;

	if(!confirm("Are you sure?")) {
		return;
	}

	// ajax - quiz inquiry update
	_ajax.post(
		// _url
		_link.get_link(_link.API_UPDATE_USER_QUIZ)
		// _param_obj
		, request_param_obj
		// _delegate_after_job_done
		, _obj.get_delegate(
			// delegate_func
			function(data){
				
				if(PROPS.IS_DEBUG_MODE == PROPS.PARAM_SET.YES) {
					console.log("EVENT_TYPE_DELETE_QUIZ / data :: ",data);
				}

				if(!data.SUCCESS) {
					console.log("Error! / #1183");
				}

				_link.refresh_post(
					_param
					.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
					.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)

					.get(PROPS.PARAM_SET.QUIZ_REGION, PROPS.QUIZ_REGION)
					.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, PROPS.QUIZ_LANGUAGE)
					.get(PROPS.PARAM_SET.QUIZ_CATEGORY, PROPS.QUIZ_CATEGORY)
					.get(PROPS.PARAM_SET.QUIZ_STATUS, PROPS.QUIZ_STATUS)

					.get(PROPS.PARAM_SET.SEARCH_KEYWORD, PROPS.SEARCH_KEYWORD)
					.get(PROPS.PARAM_SET.ROW_CNT, PROPS.ROW_CNT)
				);
			},
			// delegate_scope
			this
		)
	); // ajax done.

});
// SET EVENT - UPDATE QUIZ STATUS - DONE






// 권한 검사. Admin과 Employee만 수정 가능.
if(PROPS.USER_PERMISSION === PROPS.PARAM_SET.USER_PERMISSION_USER) {
	abc_view_input.disable();
	abc_image_input.disable();
	abc_checkbox_input.disable();
	abc_button_chain.disable();
}


// 페이지네이션 이벤트
var pagination_jq = $("ul.pagination");
var pages_jq = pagination_jq.find("li");
for(var idx=0;idx < pages_jq.length; idx++) {
	var page_jq = $(pages_jq[idx]);

	// set event
	page_jq.click(function(e){

		var self_jq = $(this);
		var cur_page = self_jq.attr("page");
		var row_cnt = parseInt(select_row_cnt_jq.val());

		_link.refresh_post(
			_param
			.get(PROPS.PARAM_SET.FACEBOOK_USER_ID, PROPS.FACEBOOK_USER_ID)
			.get(PROPS.PARAM_SET.GOOGLE_USER_ID, PROPS.GOOGLE_USER_ID)

			.get(PROPS.PARAM_SET.QUIZ_REGION, PROPS.QUIZ_REGION)
			.get(PROPS.PARAM_SET.QUIZ_LANGUAGE, PROPS.QUIZ_LANGUAGE)
			.get(PROPS.PARAM_SET.QUIZ_CATEGORY, PROPS.QUIZ_CATEGORY)
			.get(PROPS.PARAM_SET.QUIZ_STATUS, PROPS.QUIZ_STATUS)

			.get(PROPS.PARAM_SET.SEARCH_KEYWORD, PROPS.SEARCH_KEYWORD)
			.get(PROPS.PARAM_SET.ROW_CNT, row_cnt)
			.get(PROPS.PARAM_SET.PAGE_NUM, cur_page)
		);
	});
}

</script>
</body>
</html>


