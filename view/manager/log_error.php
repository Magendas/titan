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

	$ERROR_TYPE = $param->get_param_string($param->ERROR_TYPE, "");
	$PROPS->ERROR_TYPE = $ERROR_TYPE;

	// PAGINATION - INIT
	$error_total_row_cnt = $mysql_interface->select_error_log_total_cnt($PROPS);
	$preprocessor->set_pagination_total_cnt($error_total_row_cnt);
	$PROPS = $preprocessor->get_props();
	// PAGINATION - DONE

	// USER LIST FOR SELECT
	$user_list = $mysql_interface->select_user_list($PROPS);
	$PROPS->user_list = $user_list;

	$error_log_list = 
	$mysql_interface->select_error_log($PROPS);
	$PROPS->error_log_list = $error_log_list;

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
		, "[__ACTIVE_LOG_COLLASE__]"=>"in"
		, "[__ACTIVE_ERROR_LOG__]"=>"active"
		, "[__ACTIVE_ERROR_LOG_STYLE__]"=>"color:#FFFFFF !important;"
		, "[__SERVICE_ROOT__]"=>$PROPS->SERVICE_ROOT_PATH
	);
	ViewRenderer::render($PROPS->NAV_FILE_PATH,$view_render_var_arr);		

	?>
	<!-- nav ends -->

	<div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main" style="margin-top:15px;">			

		<!-- 4. QUIZ STAT -->
		<?php

			echo "<div class=\"panel panel-primary\">";
				echo "<div id=\"body_quiz_progress\" class=\"panel-body\" style=\"padding-top:0px;\">";


			// CONTROL BAR
			echo "<ul class=\"list-group\" style=\"margin-bottom:0px;margin-top:15px;\">";

				echo "<li class=\"list-group-item\" style=\"padding-bottom:44px;background-color:#f7f7f7;\">";

					// COMBO BOX - ROW CNT - INIT
					$COMBO_BOX_ROW_CNT = $PROPS->{$param->COMBO_BOX_SET}->{$param->COMBO_BOX_ROW_CNT};
					$HTML_TAG = $COMBO_BOX_ROW_CNT->{ComboBoxManager::$HTML_TAG};
					echo "$HTML_TAG";
					// COMBO BOX - ROW CNT - DONE

					// COMBO BOX - ERROR TYPE LIST - INIT
					$combobox_error_type_list = new ComboBoxManager();
					$all_error_types_list = $param->get_all_error_types();
					// ADD ONLY VALID USER
					$combobox_error_type_list->add_combobox_option("ALL");
					foreach ($all_error_types_list as $error) {
						$key = $error;
						$value = $error;
						$combobox_error_type_list->add_combobox_option($key, $value);
					}

					$selected_key = $ERROR_TYPE;
					$width = "150px";
					$combobox_id_error_type = Checker::get_hash_key("ERROR_TYPE");
					$PROPS->combobox_id_error_type = $combobox_id_error_type;
					$combobox_std_error_type = $combobox_error_type_list->get_combobox($selected_key, $width, $combobox_id_error_type);

					$HTML_TAG = $combobox_std_error_type->{ComboBoxManager::$HTML_TAG};
					echo "$HTML_TAG";
					// COMBO BOX - ERROR TYPE LIST - DONE

				echo "</li>";

			echo "</ul>";
			// CONTROL BAR				


			// PAGENATION INIT
			echo $PROPS->PAGINATION->BOOTSTRAP_TAG;
			// PAGENATION DONE
			

			// QUIZ TABLE INIT
			echo "<table id=\"quiz_table\" class=\"table table-bordered\">";
				echo "<thead>";
				echo "<tr>";

				$TABLE_COLUMN_LOG_ID = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_LOG_ID;
				$TABLE_COLUMN_LOG_INFO = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_LOG_INFO;
				$TABLE_COLUMN_LOG_MESSAGE = $PROPS->TABLE_COLUMN_SET->TABLE_COLUMN_LOG_MESSAGE;

				echo 		"<th width=\"5%\">$TABLE_COLUMN_LOG_ID</th>";
				echo 		"<th width=\"25%\">$TABLE_COLUMN_LOG_INFO</th>";
				echo 		"<th width=\"80%\">$TABLE_COLUMN_LOG_MESSAGE</th>";

				echo "</tr>"; 
				echo "</thead>";
				echo "<tbody>";

			$msg_set = new stdClass();
			foreach($error_log_list as $error_log) {

				$__id = intval($error_log->__id);
				$__browser = $error_log->__browser;
				$__ip = $error_log->__ip;
				$__os = $error_log->__os;
				$__type = $error_log->__type;
				$__msg = $error_log->__msg;
				$__time_start = $error_log->__time_start;

				$msg_set->{$__id} = $__msg;

				// $textarea_tag = "<div class=\"form-group\" style=\"margin:0px;\"><textarea style=\"resize:none;\" id=\"raw_text\" class=\"form-control\" cols=\"50\" rows=\"3\" disabled>$__msg</textarea></div>";
				$textarea_tag = "<pre id=\"json-renderer\"></pre>";
					
				echo 	"<tr id=\"content\">";

				echo 		"<td id=\"__id\"><small>$__id</small></td>";
				echo 		"<td id=\"__info\"><small>$__os / $__browser / $__ip<br/>$__type<br/>$__time_start</small></td>";
				echo 		"<td id=\"__msg\" log_id=\"$__id\">$textarea_tag</td>";
				
				// echo 		"<td id=\"__id\">$__id</td>";

				// echo 		"<td id=\"__browser\">$__browser</td>";
				// echo 		"<td id=\"__ip\">$__ip</td>";
				// echo 		"<td id=\"__os\">$__os</td>";

				// echo 		"<td id=\"__type\">$__type</td>";
				// echo 		"<td id=\"__time_start\">$__time_start</td>";
				// echo 		"<td id=\"__msg\">$textarea_tag</td>";

				echo 	"</tr>";
			}
			$PROPS->msg_set = $msg_set;

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

// json viewer setting
var row_msg_jq_arr = $("td#__msg");
for(var idx=0; idx < row_msg_jq_arr.length; idx++) {
	var row_msg_jq = $(row_msg_jq_arr[idx]);
	var log_id = parseInt(row_msg_jq.attr("log_id"));
	var msg = PROPS.msg_set[log_id];
	var msg_obj = $.parseJSON(msg);

	row_msg_jq.find('#json-renderer').jsonViewer(msg_obj,{collapsed:true});
	//row_msg_jq_arr
}

// on change select event - row cnt
var COMBO_BOX_ID = PROPS.COMBO_BOX_SET.COMBO_BOX_ROW_CNT.COMBO_BOX_ID;
var select_category_jq = $("select#" + COMBO_BOX_ID);
select_category_jq.change(function(){
	var selected_value = $(this).val();

	_link.refresh_post(
		_param
		.get(PROPS.PARAM_SET.ROW_CNT, selected_value)
	);

});
// on change select event - error type
var COMBO_BOX_ID = PROPS.combobox_id_error_type;
var select_error_type_jq = $("select#" + COMBO_BOX_ID);

console.log("select_error_type_jq ::: ",select_error_type_jq);

select_error_type_jq.change(function(){

	var selected_value = $(this).val();

	_link.refresh_post(
		_param
		.get(PROPS.PARAM_SET.ERROR_TYPE, selected_value)
	);

});


</script>
</body>
</html>


