<?php

class ActionTemplate{

	public static function get_quiz_list($table_title="", $table_row_arr=null) {

		if(empty($table_row_arr)) {
			return;
		}

		$param = new ParamManager();

		// 0. New Table
		$action_table = 
		new ActionCollection(
			// $action_name=""
			"$table_title"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_TABLE
		);		

		$first_table_row_obj = $table_row_arr[0];
		$attr_names = array();
		foreach($first_table_row_obj as $key => $value ) {
			array_push($attr_names, $key);
		}
		$category = $first_table_row_obj->{"__category"};

		for($idx=0;$idx < count($attr_names); $idx++) {

			$attr_name = $attr_names[$idx];
			if(strcmp($attr_name, "__thumbnail") == 0) {
				continue;
			}
			if(strcmp($attr_name, "__category") == 0) {
				continue;
			}
			if(strcmp($attr_name, "__img_link") == 0 && strcmp($category, "S_IMAGE") != 0) {
				continue;
			}


			// 0-1. Column List
			$action_table_column_list = 
			new ActionCollection(
				// $action_name=""
				$table_title . "_column_" . $idx . $attr_name
				// $action_collection_type=1
				, ActionCollection::$ACTION_COLLECTION_TYPE_LIST
			);
			$action_table->add_child($action_table_column_list);

			// set title
			$title = "";
			if(strcmp($attr_name, "__id") == 0) {
				$title = "ID";
			} else if(strcmp($attr_name, "__quiz_num") == 0) {
				$title = "QUIZ NUM";
			} else if(strcmp($attr_name, "__inquiry") == 0) {
				$title = "INQUIRY";
			} else if(strcmp($attr_name, "__desc") == 0) {
				$title = "DESC";
			} else if(strcmp($attr_name, "__img_link") == 0) {
				$title = "IMAGE";
			} else if(strcmp($attr_name, "__answer") == 0) {
				$title = "ANSWER";
			} else if(strcmp($attr_name, "__percent_correct") == 0) {
				$title = "RATIO";
			}
			$action_table_column_list_field =  
			new ActionItem(
				// $action_name="New Item"
				$title
				// $action_item_type=1
				, ActionItem::$TITLE_ONLY_FIXED
			);
			$action_table_column_list->add_child($action_table_column_list_field);	


			for($inner_idx=0;$inner_idx < count($table_row_arr); $inner_idx++) {
				$table_row_obj = $table_row_arr[$inner_idx];

				// horizontal --> verticals

				$value = $table_row_obj->{$attr_name};
				if(empty($value)) {
					$value = " ";
				}
				if(strcmp($attr_name, "__percent_correct") == 0) {
					$value = $value . "%";
				}

				// echo "\$value ::: $value<br/>";
				$action_item_type = ActionItem::$TITLE_ONLY;
				if(	strcmp($attr_name, "__id") == 0 || 
					strcmp($attr_name, "__category") == 0 ||
					strcmp($attr_name, "__quiz_num") == 0 ||
					strcmp($attr_name, "__thumbnail") == 0 ||
					strcmp($attr_name, "__percent_correct") == 0 ) {

					$action_item_type = ActionItem::$TITLE_ONLY_FIXED;

				} else if(strcmp($attr_name, "__answer") == 0) {

					$action_item_type = ActionItem::$SELECT_BOX;

				}

				// Column List Field - Timer
				$action_table_column_list_field =  
				new ActionItem(
					// $action_name="New Item"
					$value
					// $action_item_type=1
					, $action_item_type
				);



				$context = new stdClass();

				$context->{$param->QUIZ_CATEGORY} = $table_row_obj->__category;
				$context->{$param->QUIZ_ID} = $table_row_obj->__id;
				$context->{$param->ATTR_NAME} = $attr_name;
				if(strcmp($attr_name, "__answer") == 0) {
					$context->{$param->SELECT_OPTION_VALUES} = array($param->QUIZ_ANSWER_O, $param->QUIZ_ANSWER_X);
				}
				
				$action_table_column_list_field->set_context(json_encode($context));

				$action_table_column_list->add_child($action_table_column_list_field);	
				
			}

		}

		// wonder.jung

		// • 각 카테고리별로 그동안 풀어진 수, 정답률, 신고 내용 표시

		// 경고 표시

		// • x번 이상 맞춰진 문제가 너무 정답률이 낮거나 높으면
		// ( ~10%, 10~20%, 80~90%, 90%~)경고 표시
		// --> 해당 문제가 몇번 풀렸는지 조회하는 쿼리가 필요함.

		// • 부정적인 신고 ( 재미없음, 오류, 너무 어려움, 너무 쉬움 )가 x번 이상이면 경고 표시
		// --> 해당 문제가 받은 신고의 횟수에 대해 조회하는 쿼리 필요함.

		// • 긍정적인 신고 ( 재미있음 ) 표시가 많으면 스마일 표시		
		// --> 해당 문제가 받은 긍정 피드백의 횟수에 대해 조회하는 쿼리 필요함.

		return $action_table->get_std_obj();

	}















































































	public static function get_TM_Default($hh_mm_begins="", $root_action_obj_name="") {

		if(TimeManager::is_not_valid_time_format_hh24_mm($hh_mm_begins)) {
			echo "TimeManager::is_not_valid_time_format_hh24_mm(\$hh_mm_begins)<br/>";
			return;
		}
		if(empty($root_action_obj_name)){
			echo "empty(\$root_action_obj_name)";
			return;
		}

		$param = new ToastmasterParamManager();

		// 0. Action obj로 리스트를 구성합니다.
		$root_action_obj = 
		new ActionCollection(
			// $action_name=""
			$root_action_obj_name
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_LIST
		);

		// 1.Greeting

		$time_init_sec = TimeManager::convert_hh24_mm_to_seconds($hh_mm_begins);
		$time_init_hh24_mm = TimeManager::convert_seconds_to_hh24_mm($time_init_sec);
		$time_offset_sec = 0;
		$time_next_sec = $time_init_sec + $time_offset_sec;
		$time_next_hh24_mm = TimeManager::convert_seconds_to_hh24_mm($time_next_sec);

		if(TimeManager::is_not_valid_time_format_hh24_mm($time_init_hh24_mm)) {
			echo "TimeManager::is_not_valid_time_format_hh24_mm(\$time_init_hh24_mm)<br/>";
			return;
		}

		if(TimeManager::is_not_valid_time_format_hh24_mm($time_next_hh24_mm)) {
			echo "TimeManager::is_not_valid_time_format_hh24_mm(\$time_next_hh24_mm)<br/>";
			return;
		}

		$child_action_item =  
		new ActionItemTitleNTimeHHMM(
			// $action_name=""
			"Greetings"
			// $time_str_hhmm_init=""
			, $time_init_hh24_mm
			// $time_str_hhmm_specific=""
			, $time_next_hh24_mm
		);
		$root_action_obj->add_child($child_action_item);

		// 1-1. Sublist
		$child_action_sub_list = 
		new ActionCollection(
			// $action_name=""
			"Greetings - Sublist"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_LIST
		);
		$child_action_item->add_child($child_action_sub_list);

		// 1-1-1. Welcome guests / Guest introduction
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"New Item"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
			// $is_shy=0
			, 1
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);		






		// 2.Call meeting to order [Chairperson]

		$time_offset_sec = 60 * 10; // 10 mins / "19:50"
		$time_next_sec = $time_next_sec + $time_offset_sec;
		$time_next_hh24_mm = TimeManager::convert_seconds_to_hh24_mm($time_next_sec);

		if(TimeManager::is_not_valid_time_format_hh24_mm($time_next_hh24_mm)) {
			echo "TimeManager::is_not_valid_time_format_hh24_mm(\$time_next_hh24_mm)<br/>";
			return;
		}

		$child_action_item =  
		new ActionItemTitleNTimeHHMM(
			// $action_name=""
			"Call meeting to order [Chairperson]"
			// $time_str_hhmm_init="" / "19:40"
			, $time_init_hh24_mm
			// $time_str_hhmm_specific="" / "19:50"
			, $time_next_hh24_mm
		);
		$root_action_obj->add_child($child_action_item);

		// 2-1. Sublist
		$child_action_sub_list = 
		new ActionCollection(
			// $action_name=""
			"Call meeting to order [Chairperson] - Sublist"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_LIST
		);
		$child_action_item->add_child($child_action_sub_list);

		// 2-1-1. Welcome guests / Guest introduction
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Welcome guests / Guest introduction"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);

		// 2-1-2. Cite the mission of Toastmasters
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Cite the mission of Toastmasters"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);

		// 2-1-3. Cite the mission of Toastmasters
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Fill Vacant functions & Share CL process"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);

		// 2-1-4. Cite the mission of Toastmasters
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Introduce Toastmaster"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);






		// 20:00
		// 3.Theme & Role Introduction [Toastmaster]

		$time_offset_sec = 60 * 10; // 10 mins / "19:40" / "19:50" --> "20:00"
		$time_next_sec = $time_next_sec + $time_offset_sec;
		$time_next_hh24_mm = TimeManager::convert_seconds_to_hh24_mm($time_next_sec); // $time_init_hh24_mm

		if(TimeManager::is_not_valid_time_format_hh24_mm($time_next_hh24_mm)) {
			echo "TimeManager::is_not_valid_time_format_hh24_mm(\$time_next_hh24_mm)<br/>";
			return;
		}

		$child_action_item =  
		new ActionItemTitleNTimeHHMM(
			// $action_name=""
			"Theme & Role Introduction [Toastmaster]"
			// $time_str_hhmm_init="" / "19:40"
			, $time_init_hh24_mm
			// $time_str_hhmm_specific="" / "20:00"
			, $time_next_hh24_mm
		);
		$root_action_obj->add_child($child_action_item);

		// 3-1. Sublist
		$child_action_sub_list = 
		new ActionCollection(
			// $action_name=""
			"Theme & Role Introduction [Toastmaster] - Sublist"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_LIST
		);
		$child_action_item->add_child($child_action_sub_list);

		// 3-1-1. Opening remarks ( Introduce the theme )
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Opening remarks ( Introduce the theme )"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);

		// 3-1-2. Introduce roles : General Evaluator ( < 1 min ), Timer, Ah Counter, Grammarian ( < 1 min each ), Word & Quote master ( < 2 min )
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Introduce roles : General Evaluator ( < 1 min ), Timer, Ah Counter, Grammarian ( < 1 min each ), Word & Quote master ( < 2 min )"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);

		// add_on inner table
		// 3-1-2-1. Today's Role
		$child_action_sub_list_child_action_item_sub_table = 
		new ActionCollection(
			// $action_name=""
			"Today's Role"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_TABLE
		);
		$child_action_sub_list_child_action_item->push_add_on($child_action_sub_list_child_action_item_sub_table);


		// 3-1-2-1-1. Column List - Right Role Name List
		$child_action_sub_list_child_action_item_sub_table_column_list = 
		new ActionCollection(
			// $action_name=""
			"Left Role Name"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_LIST
		);
		$child_action_sub_list_child_action_item_sub_table->add_child($child_action_sub_list_child_action_item_sub_table_column_list);

		// 3-1-2-1-1-1. Column List Field - Toastmaster
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"Toastmaster"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_FIXED
		);
		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);
		// 3-1-2-1-1-2. Column List Field - Grammarian
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"Grammarian"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_FIXED
		);
		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);
		// 3-1-2-1-1-3. Column List Field - Timer
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"Timer"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_FIXED
		);
		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);
		// 3-1-2-1-1-4. Column List Field - Table Topic Master
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"Table Topic Master"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_FIXED
		);
		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);


		// 3-1-2-1-2. Column List - Right Role Name List
		$child_action_sub_list_child_action_item_sub_table_column_list = 
		new ActionCollection(
			// $action_name=""
			"Left Member Name"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_LIST
		);
		$child_action_sub_list_child_action_item_sub_table->add_child($child_action_sub_list_child_action_item_sub_table_column_list);

		// 3-1-2-1-2-1. Column List Field - Toastmaster
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"NOT ASSIGNED"
			// $action_item_type=1
			, ActionItem::$SELECT_BOX_PERSON_NAME
		);

		// DB interface prop - Toastmaster / 이 정보를 외부에서 변경하는 필요가 있음.
		$context = new stdClass();

		$context->{$param->ACTION_DB_UPDATE_MSG} = $param->IS_UPDATE_TODAY_ROLE;
		$context->{$param->ROLE_ID} = $param->ROLE_ID_TOASTMASTER;
		$context->{$param->MEMBER_HASH_KEY} = "";
		$child_action_sub_list_child_action_item_sub_table_column_list_field->set_context(json_encode($context));

		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);

		// 3-1-2-1-2-2. Column List Field - Grammarian
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"NOT ASSIGNED"
			// $action_item_type=1
			, ActionItem::$SELECT_BOX_PERSON_NAME
		);

		// DB interface prop - Grammarian
		$context = new stdClass();
		$context->{$param->ACTION_DB_UPDATE_MSG} = $param->IS_UPDATE_TODAY_ROLE;
		$context->{$param->ROLE_ID} = $param->ROLE_ID_GRAMMARIAN;
		$context->{$param->MEMBER_HASH_KEY} = "";
		$child_action_sub_list_child_action_item_sub_table_column_list_field->set_context(json_encode($context));

		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);

		// 3-1-2-1-2-3. Column List Field - Timer
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"NOT ASSIGNED"
			// $action_item_type=1
			, ActionItem::$SELECT_BOX_PERSON_NAME
		);

		// DB interface prop - Timer
		$context = new stdClass();
		$context->{$param->ACTION_DB_UPDATE_MSG} = $param->IS_UPDATE_TODAY_ROLE;
		$context->{$param->ROLE_ID} = $param->ROLE_ID_TIMER;
		$context->{$param->MEMBER_HASH_KEY} = "";
		$child_action_sub_list_child_action_item_sub_table_column_list_field->set_context(json_encode($context));

		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);

		// 3-1-2-1-2-4. Column List Field - Table Topic Master
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"NOT ASSIGNED"
			// $action_item_type=1
			, ActionItem::$SELECT_BOX_PERSON_NAME
		);

		// DB interface prop - Table Topic Master
		$context = new stdClass();
		$context->{$param->ACTION_DB_UPDATE_MSG} = $param->IS_UPDATE_TODAY_ROLE;
		$context->{$param->ROLE_ID} = $param->ROLE_ID_TABLE_TOPIC_MASTER;
		$context->{$param->MEMBER_HASH_KEY} = "";
		$child_action_sub_list_child_action_item_sub_table_column_list_field->set_context(json_encode($context));

		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);


		// 3-1-2-1-3. Column List - Right Role Name List
		$child_action_sub_list_child_action_item_sub_table_column_list = 
		new ActionCollection(
			// $action_name=""
			"Right Role Name"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_LIST
		);
		$child_action_sub_list_child_action_item_sub_table->add_child($child_action_sub_list_child_action_item_sub_table_column_list);

		// 3-1-2-1-3-1. Column List Field - General Evaluator
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"General Evaluator"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_FIXED
		);
		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);
		// 3-1-2-1-3-2. Column List Field - Ah & Vote Counter
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"Ah & Vote Counter"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_FIXED
		);
		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);
		// 3-1-2-1-3-3. Column List Field - Word & Quote Master
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"Word & Quote Master"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_FIXED
		);
		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);
		// 3-1-2-1-3-4. Column List Field - Mini Debate Master
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"Mini Debate Master"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_FIXED
		);
		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);		


		// 3-1-2-1-4. Column List - Right Role Name List
		$child_action_sub_list_child_action_item_sub_table_column_list = 
		new ActionCollection(
			// $action_name=""
			"Right Member Name"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_LIST
		);
		$child_action_sub_list_child_action_item_sub_table->add_child($child_action_sub_list_child_action_item_sub_table_column_list);

		// 3-1-2-1-4-1. Column List Field - General Evaluator
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"NOT ASSIGNED"
			// $action_item_type=1
			, ActionItem::$SELECT_BOX_PERSON_NAME
		);

		// DB interface prop - General Evaluator
		$context = new stdClass();
		$context->{$param->ACTION_DB_UPDATE_MSG} = $param->IS_UPDATE_TODAY_ROLE;
		$context->{$param->ROLE_ID} = $param->ROLE_ID_GENERAL_EVALUATOR;
		$context->{$param->MEMBER_HASH_KEY} = "";
		$child_action_sub_list_child_action_item_sub_table_column_list_field->set_context(json_encode($context));

		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);

		// 3-1-2-1-4-2. Column List Field - Ah & Vote Counter
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"NOT ASSIGNED"
			// $action_item_type=1
			, ActionItem::$SELECT_BOX_PERSON_NAME
		);

		// DB interface prop - Ah & Vote Counter
		$context = new stdClass();
		$context->{$param->ACTION_DB_UPDATE_MSG} = $param->IS_UPDATE_TODAY_ROLE;
		$context->{$param->ROLE_ID} = $param->ROLE_ID_AH_COUNTER;
		$context->{$param->MEMBER_HASH_KEY} = "";
		$child_action_sub_list_child_action_item_sub_table_column_list_field->set_context(json_encode($context));

		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);


		// 3-1-2-1-4-3. Column List Field - Word & Quote Master
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"NOT ASSIGNED"
			// $action_item_type=1
			, ActionItem::$SELECT_BOX_PERSON_NAME
		);

		// DB interface prop - Word & Quote Master
		$context = new stdClass();
		$context->{$param->ACTION_DB_UPDATE_MSG} = $param->IS_UPDATE_TODAY_ROLE;
		$context->{$param->ROLE_ID} = $param->ROLE_ID_WORD_N_QUOTE_MASTER;
		$context->{$param->MEMBER_HASH_KEY} = "";
		$child_action_sub_list_child_action_item_sub_table_column_list_field->set_context(json_encode($context));

		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);


		// 3-1-2-1-4-4. Column List Field - Mini Debate Master
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"NOT ASSIGNED"
			// $action_item_type=1
			, ActionItem::$SELECT_BOX_PERSON_NAME
		);

		// DB interface prop - Mini Debate Master
		$context = new stdClass();
		$context->{$param->ACTION_DB_UPDATE_MSG} = $param->IS_UPDATE_TODAY_ROLE;
		$context->{$param->ROLE_ID} = $param->ROLE_ID_MINI_DEBATE_MASTER;
		$context->{$param->MEMBER_HASH_KEY} = "";
		$child_action_sub_list_child_action_item_sub_table_column_list_field->set_context(json_encode($context));

		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);




		// add_on inner table
		// 3-1-2-2. Today's Role
		$child_action_sub_list_child_action_item_sub_table = 
		new ActionCollection(
			// $action_name=""
			"Word & Quote"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_TABLE
		);
		$child_action_sub_list_child_action_item->push_add_on($child_action_sub_list_child_action_item_sub_table);

		// 3-1-2-2-1. Column List - Title Column List
		$child_action_sub_list_child_action_item_sub_table_column_list = 
		new ActionCollection(
			// $action_name=""
			"Title Column List"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_LIST
		);
		$child_action_sub_list_child_action_item_sub_table->add_child($child_action_sub_list_child_action_item_sub_table_column_list);

		// wonder.jung
		// 3-1-2-2-1-1. Column List Field - Word
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"Word"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_FIXED
		);

		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);

		// wonder.jung
		// 3-1-2-2-1-2. Column List Field - Word Desc
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"Word Desc"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_FIXED
		);

		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);

		// wonder.jung
		// 3-1-2-2-1-3. Column List Field - Quote
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"Quote"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_FIXED
		);

		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);


		// 3-1-2-2-2. Column List - Speech Title Column List
		$child_action_sub_list_child_action_item_sub_table_column_list = 
		new ActionCollection(
			// $action_name=""
			"Text Column List"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_LIST
		);
		$child_action_sub_list_child_action_item_sub_table->add_child($child_action_sub_list_child_action_item_sub_table_column_list);

		// 3-1-2-2-2-1. Column List Field - Word Text
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"NOT ASSIGNED"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY
		);

		// DB interface prop
		$context = new stdClass();
		$context->{$param->ACTION_DB_UPDATE_MSG} = $param->IS_UPDATE_WORD_N_QUOTE_WORD_ONLY;
		$context->{$param->WORD} = $param->YES;
		$child_action_sub_list_child_action_item_sub_table_column_list_field->set_context(json_encode($context));

		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);

		// 3-1-2-2-2-2. Column List Field - Word Desc Text
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"NOT ASSIGNED"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY
		);

		// DB interface prop
		$context = new stdClass();
		$context->{$param->ACTION_DB_UPDATE_MSG} = $param->IS_UPDATE_WORD_N_QUOTE_WORD_DESC_ONLY;
		$context->{$param->WORD_DESC} = $param->YES;
		$child_action_sub_list_child_action_item_sub_table_column_list_field->set_context(json_encode($context));

		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);

		// 3-1-2-2-2-3. Column List Field - Quote Desc Text
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"NOT ASSIGNED"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY
		);

		// DB interface prop
		$context = new stdClass();
		$context->{$param->ACTION_DB_UPDATE_MSG} = $param->IS_UPDATE_WORD_N_QUOTE_QUOTE_ONLY;
		$context->{$param->QUOTE} = $param->YES;
		$child_action_sub_list_child_action_item_sub_table_column_list_field->set_context(json_encode($context));
		
		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);		

		// 20:00







		// 20:15
		// 4.Table Topics Session [ Table Topic Master ]

		$time_offset_sec = 60 * 15; // 15 mins / "19:40" / "20:00" --> "20:15"
		$time_next_sec = $time_next_sec + $time_offset_sec;
		$time_next_hh24_mm = TimeManager::convert_seconds_to_hh24_mm($time_next_sec); // $time_init_hh24_mm
		if(TimeManager::is_not_valid_time_format_hh24_mm($time_next_hh24_mm)) {
			echo "TimeManager::is_not_valid_time_format_hh24_mm(\$time_next_hh24_mm)<br/>";
			return;
		}

		$child_action_item =  
		new ActionItemTitleNTimeHHMM(
			// $action_name=""
			"Table Topics Session [ Table Topic Master ]"
			// $time_str_hhmm_init="" / "19:40"
			, $time_init_hh24_mm
			// $time_str_hhmm_specific="" / "20:15"
			, $time_next_hh24_mm
		);
		$root_action_obj->add_child($child_action_item);

		// 4-1. Sublist
		$child_action_sub_list = 
		new ActionCollection(
			// $action_name=""
			"Table Topics Session [ Table Topic Master ] - Sublist"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_LIST
		);
		$child_action_item->add_child($child_action_sub_list);

		// 4-1-1. Explain the rules and theme
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Explain the rules and theme"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);

		// 4-1-2. 1 to 2 minutes impromptu speaking
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"1 to 2 minutes impromptu speaking"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);

		// 4-1-3. Timer‘s Report
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Timer‘s Report"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);

		// 4-1-4. Vote for the Best Table Topic Speaker
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Vote for the Best Table Topic Speaker"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);
		// 20:15









		// 20:25
		// 5. Prepared Speech Session [ Toastmaster ]

		$time_offset_sec = 60 * 10; // 10 mins / "19:40" / "20:15" --> "20:25"
		$time_next_sec = $time_next_sec + $time_offset_sec;
		$time_next_hh24_mm = TimeManager::convert_seconds_to_hh24_mm($time_next_sec); // $time_init_hh24_mm
		if(TimeManager::is_not_valid_time_format_hh24_mm($time_next_hh24_mm)) {
			echo "TimeManager::is_not_valid_time_format_hh24_mm(\$time_next_hh24_mm)<br/>";
			return;
		}

		$child_action_item =  
		new ActionItemTitleNTimeHHMM(
			// $action_name=""
			"Prepared Speech Session [ Toastmaster ]"
			// $time_str_hhmm_init="" / "19:40"
			, $time_init_hh24_mm
			// $time_str_hhmm_specific="" / "20:25"
			, $time_next_hh24_mm
		);
		$root_action_obj->add_child($child_action_item);

		// 5-1. Sublist
		$child_action_sub_list = 
		new ActionCollection(
			// $action_name=""
			"Prepared Speech Session [ Toastmaster ] - Sublist"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_LIST
		);
		$child_action_item->add_child($child_action_sub_list);

		// 5-1-1. Introduce speakers ( Speaker, Project Number, Topic, Evaluator, Speech Objectives, Time )
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Introduce speakers ( Speaker, Project Number, Topic, Evaluator, Speech Objectives, Time )"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);


		// add_on inner table
		// 5-1-1-1. Today's Role
		$child_action_sub_list_child_action_item_sub_table = 
		new ActionCollection(
			// $action_name=""
			"Today's Speech"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_TABLE
		);
		$child_action_sub_list_child_action_item->push_add_on($child_action_sub_list_child_action_item_sub_table);

		// 5-1-1-1-1. Column List - Project Column List
		$child_action_sub_list_child_action_item_sub_table_column_list = 
		new ActionCollection(
			// $action_name=""
			"Project Column List"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_LIST
		);
		$child_action_sub_list_child_action_item_sub_table->add_child($child_action_sub_list_child_action_item_sub_table_column_list);

		// 5-1-1-1-1-1. Column List Field - Speech Project
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"Project"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_FIXED
		);
		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);

		// 5-1-1-1-1-2. Column List Field - Speech Project Editable
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"NOT ASSIGNED"
			// $action_item_type=1
			, ActionItem::$SELECT_BOX_ADDABLE
		);

		// DB interface prop
		$context = new stdClass();
		$context->{$param->ACTION_DB_INSERT_MSG} = $param->IS_INSERT_SPEECH;
		$context->{$param->ACTION_DB_UPDATE_MSG} = $param->IS_UPDATE_SPEECH_PROJECT;
		$context->{$param->ACTION_DB_DELETE_MSG} = $param->IS_DELETE_SPEECH;
		$context->{$param->SPEECH_ID} = -1;
		$context->{$param->SPEECH_PROJECT_ID} = -1;
		$child_action_sub_list_child_action_item_sub_table_column_list_field->set_context(json_encode($context));

		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);


		// 5-1-1-1-2. Column List - Speech Title Column List
		$child_action_sub_list_child_action_item_sub_table_column_list = 
		new ActionCollection(
			// $action_name=""
			"Speech Title Column List"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_LIST
		);
		$child_action_sub_list_child_action_item_sub_table->add_child($child_action_sub_list_child_action_item_sub_table_column_list);

		// 5-1-1-1-2-1. Column List Field - Speech Title
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"Title"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_FIXED
		);
		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);

		// 5-1-1-1-2-2. Column List Field - Speech Title Editable
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"NOT ASSIGNED"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);

		// DB interface prop
		$context = new stdClass();
		$context->{$param->ACTION_DB_INSERT_MSG} = $param->IS_INSERT_SPEECH;
		$context->{$param->ACTION_DB_UPDATE_MSG} = $param->IS_UPDATE_SPEECH_TITLE;
		$context->{$param->ACTION_DB_DELETE_MSG} = $param->IS_DELETE_SPEECH;
		$context->{$param->SPEECH_ID} = -1;
		$context->{$param->SPEECH_TITLE} = "NO TITLE";
		$child_action_sub_list_child_action_item_sub_table_column_list_field->set_context(json_encode($context));

		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);


		// 5-1-1-1-3. Column List - Speaker Column List
		$child_action_sub_list_child_action_item_sub_table_column_list = 
		new ActionCollection(
			// $action_name=""
			"Speaker Column List"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_LIST
		);
		$child_action_sub_list_child_action_item_sub_table->add_child($child_action_sub_list_child_action_item_sub_table_column_list);

		// 5-1-1-1-3-1. Column List Field - Speaker
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"Speaker"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_FIXED
		);
		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);

		// 5-1-1-1-3-2. Column List Field - Speaker Editable
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"NOT ASSIGNED"
			// $action_item_type=1
			, ActionItem::$SELECT_BOX_ADDABLE_PERSON_NAME
		);

		// DB interface prop
		$context = new stdClass();
		$context->{$param->ACTION_DB_INSERT_MSG} = $param->IS_INSERT_SPEECH;
		$context->{$param->ACTION_DB_UPDATE_MSG} = $param->IS_UPDATE_SPEECH_SPEAKER;
		$context->{$param->ACTION_DB_DELETE_MSG} = $param->IS_DELETE_SPEECH;
		$context->{$param->SPEECH_ID} = -1;
		$context->{$param->MEMBER_HASH_KEY} = "";
		$child_action_sub_list_child_action_item_sub_table_column_list_field->set_context(json_encode($context));

		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);


		// 5-1-1-1-4. Column List - Evaluator Column List
		$child_action_sub_list_child_action_item_sub_table_column_list = 
		new ActionCollection(
			// $action_name=""
			"Evaluator Column List"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_LIST
		);
		$child_action_sub_list_child_action_item_sub_table->add_child($child_action_sub_list_child_action_item_sub_table_column_list);

		// 5-1-1-1-4-1. Column List Field - Evaluator
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"Evaluator"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_FIXED
		);
		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);

		// 5-1-1-1-4-2. Column List Field - Evaluator Editable
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"NOT ASSIGNED"
			// $action_item_type=1
			, ActionItem::$SELECT_BOX_ADDABLE_PERSON_NAME
		);

		// DB interface prop
		$context = new stdClass();
		$context->{$param->ACTION_DB_INSERT_MSG} = $param->IS_INSERT_SPEECH;
		$context->{$param->ACTION_DB_UPDATE_MSG} = $param->IS_UPDATE_SPEECH_EVALUATOR;
		$context->{$param->ACTION_DB_DELETE_MSG} = $param->IS_DELETE_SPEECH;
		$context->{$param->SPEECH_ID} = -1;
		$context->{$param->MEMBER_HASH_KEY} = "";
		$child_action_sub_list_child_action_item_sub_table_column_list_field->set_context(json_encode($context));

		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);		








		// 5-1-2. Timer‘s Report
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Timer‘s Report"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);

		// 5-1-3. Vote for the Best Prepared Speech Speaker
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Vote for the Best Prepared Speech Speaker"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);

		// 20:25










		// 21:00
		// 6. Break

		$time_offset_sec = 60 * 35; // 35 mins / "19:40" / "20:25" --> "21:00"
		$time_next_sec = $time_next_sec + $time_offset_sec;
		$time_next_hh24_mm = TimeManager::convert_seconds_to_hh24_mm($time_next_sec); // $time_init_hh24_mm
		if(TimeManager::is_not_valid_time_format_hh24_mm($time_next_hh24_mm)) {
			echo "TimeManager::is_not_valid_time_format_hh24_mm(\$time_next_hh24_mm)<br/>";
			return;
		}

		$child_action_item =  
		new ActionItemTitleNTimeHHMM(
			// $action_name=""
			"Break"
			// $time_str_hhmm_init="" / "19:40"
			, $time_init_hh24_mm
			// $time_str_hhmm_specific="" / "21:00"
			, $time_next_hh24_mm
		);
		$root_action_obj->add_child($child_action_item);

		// 6-1. Sublist
		$child_action_sub_list = 
		new ActionCollection(
			// $action_name=""
			"Break - Sublist"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_LIST
		);
		$child_action_item->add_child($child_action_sub_list);

		// 6-1-1. Complete Feedback to prepared speakers
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Complete Feedback to prepared speakers"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);

		// 21:00










		// 21:10
		// 7. Break

		$time_offset_sec = 60 * 10; // 10 mins / "19:40" / "21:00" --> "21:10"
		$time_next_sec = $time_next_sec + $time_offset_sec;
		$time_next_hh24_mm = TimeManager::convert_seconds_to_hh24_mm($time_next_sec); // $time_init_hh24_mm
		if(TimeManager::is_not_valid_time_format_hh24_mm($time_next_hh24_mm)) {
			echo "TimeManager::is_not_valid_time_format_hh24_mm(\$time_next_hh24_mm)<br/>";
			return;
		}

		$child_action_item =  
		new ActionItemTitleNTimeHHMM(
			// $action_name=""
			"Mini debate session"
			// $time_str_hhmm_init="" / "19:40"
			, $time_init_hh24_mm
			// $time_str_hhmm_specific="" / "21:10"
			, $time_next_hh24_mm
		);
		$root_action_obj->add_child($child_action_item);
		// 7-1. Sublist
		$child_action_sub_list = 
		new ActionCollection(
			// $action_name=""
			"Mini debate session - Sublist"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_LIST
		);
		$child_action_item->add_child($child_action_sub_list);

		// 7-1-1. Welcome guests / Guest introduction
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"New Item"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
			// $is_shy=0
			, 1
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);			
		// 21:10		










		// 21:20
		// 8. Evaluation Session [ General Evaluator ]

		$time_offset_sec = 60 * 10; // 10 mins / "19:40" / "21:10" --> "21:20"
		$time_next_sec = $time_next_sec + $time_offset_sec;
		$time_next_hh24_mm = TimeManager::convert_seconds_to_hh24_mm($time_next_sec); // $time_init_hh24_mm
		if(TimeManager::is_not_valid_time_format_hh24_mm($time_next_hh24_mm)) {
			echo "TimeManager::is_not_valid_time_format_hh24_mm(\$time_next_hh24_mm)<br/>";
			return;
		}

		$child_action_item =  
		new ActionItemTitleNTimeHHMM(
			// $action_name=""
			"Evaluation Session [ General Evaluator ]"
			// $time_str_hhmm_init="" / "19:40"
			, $time_init_hh24_mm
			// $time_str_hhmm_specific="" / "21:20"
			, $time_next_hh24_mm
		);
		$root_action_obj->add_child($child_action_item);

		// 8-1. Sublist
		$child_action_sub_list = 
		new ActionCollection(
			// $action_name=""
			"Evaluation Session [ General Evaluator ] - Sublist"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_LIST
		);
		$child_action_item->add_child($child_action_sub_list);

		// 8-1-1. Introduces Evaluators
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Introduces Evaluators"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);

		// 8-1-2. Timer‘s Report & Vote
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Timer‘s Report & Vote"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);

		// 8-1-3. Ah Counter‘s Ah tally report
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Ah Counter‘s Ah tally report"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);

		// 8-1-4. Grammarian‘s Report
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Grammarian‘s Report"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);

		// 8-1-5. General comments on meeting ( < 3 min )
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"General comments on meeting ( < 3 min )"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);
		// 21:20







		// 21:35
		// 9. Evaluation Session [ General Evaluator ]

		$time_offset_sec = 60 * 15; // 15 mins / "19:40" / "21:20" --> "21:35"
		$time_next_sec = $time_next_sec + $time_offset_sec;
		$time_next_hh24_mm = TimeManager::convert_seconds_to_hh24_mm($time_next_sec); // $time_init_hh24_mm
		if(TimeManager::is_not_valid_time_format_hh24_mm($time_next_hh24_mm)) {
			echo "TimeManager::is_not_valid_time_format_hh24_mm(\$time_next_hh24_mm)<br/>";
			return;
		}

		$child_action_item =  
		new ActionItemTitleNTimeHHMM(
			// $action_name=""
			"Award Session [ Toastmaster ]"
			// $time_str_hhmm_init="" / "19:40"
			, $time_init_hh24_mm
			// $time_str_hhmm_specific="" / "21:35"
			, $time_next_hh24_mm
		);
		$root_action_obj->add_child($child_action_item);

		// 9-1. Sublist
		$child_action_sub_list = 
		new ActionCollection(
			// $action_name=""
			"Award Session [ Toastmaster ] - Sublist"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_LIST
		);
		$child_action_item->add_child($child_action_sub_list);

		// 9-1-1. Present award ( < 1 min )
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Present award ( < 1 min )"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);

		// 9-1-2. Closing remarks ( < 2 min )
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Closing remarks ( < 2 min )"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);		
		// 21:35







		// 21:40
		// 10. Club Business [ Chairperson ]

		$time_offset_sec = 60 * 5; // 5 mins / "19:40" / "21:35" --> "21:40"
		$time_next_sec = $time_next_sec + $time_offset_sec;
		$time_next_hh24_mm = TimeManager::convert_seconds_to_hh24_mm($time_next_sec); // $time_init_hh24_mm
		if(TimeManager::is_not_valid_time_format_hh24_mm($time_next_hh24_mm)) {
			echo "TimeManager::is_not_valid_time_format_hh24_mm(\$time_next_hh24_mm)<br/>";
			return;
		}

		$child_action_item =  
		new ActionItemTitleNTimeHHMM(
			// $action_name=""
			"Club Business [ Chairperson ]"
			// $time_str_hhmm_init="" / "19:40"
			, $time_init_hh24_mm
			// $time_str_hhmm_specific="" / "21:40"
			, $time_next_hh24_mm
		);
		$root_action_obj->add_child($child_action_item);

		// 10-1. Sublist
		$child_action_sub_list = 
		new ActionCollection(
			// $action_name=""
			"Club Business [ Chairperson ] - Sublist"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_LIST
		);
		$child_action_item->add_child($child_action_sub_list);

		// 10-1-1. Club News ( What's new? )
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Club News ( What's new? )"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);


		// add_on inner table
		// 10-1-1-1. Club News / 뉴스의 내용은 action obj 형식으로 관리해도 문제 없음.
		$child_action_sub_list_child_action_item_sub_table = 
		new ActionCollection(
			// $action_name=""
			"Club News"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_TABLE
		);
		$child_action_sub_list_child_action_item->push_add_on($child_action_sub_list_child_action_item_sub_table);

		// wonder.jung

		// 10-1-1-1-1. Column List - Project Column List
		$child_action_sub_list_child_action_item_sub_table_column_list = 
		new ActionCollection(
			// $action_name=""
			"News List"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_LIST
		);
		$child_action_sub_list_child_action_item_sub_table->add_child($child_action_sub_list_child_action_item_sub_table_column_list);

		// 10-1-1-1-1-1. Column List Field - Speech Project
		$child_action_sub_list_child_action_item_sub_table_column_list_field =  
		new ActionItem(
			// $action_name="New Item"
			"NOT ASSIGNED"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);

		// DB interface prop
		$context = new stdClass();
		$context->{$param->ACTION_DB_INSERT_MSG} = $param->IS_INSERT_NEWS;
		$context->{$param->ACTION_DB_UPDATE_MSG} = $param->IS_UPDATE_NEWS;
		$context->{$param->ACTION_DB_DELETE_MSG} = $param->IS_DELETE_NEWS;
		$context->{$param->NEWS_ID} = -1;
		$context->{$param->NEWS_CONTENTS} = "";
		$child_action_sub_list_child_action_item_sub_table_column_list_field->set_context(json_encode($context));

		$child_action_sub_list_child_action_item_sub_table_column_list->add_child($child_action_sub_list_child_action_item_sub_table_column_list_field);		




		// 10-1-2. Announcement & Role Sign-up for future meeting
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Announcement & Role Sign-up for future meeting"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);

		// 10-1-3. Guest Comments
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Guest Comments"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);

		// 10-1-4. Acknowledgement
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Acknowledgement"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);
		// 21:40	







		// 21:50
		// 11. Call Meeting to adjourn & Group photos

		$time_offset_sec = 60 * 5; // 5 mins / "19:40" / "21:40" --> "21:45"
		$time_next_sec = $time_next_sec + $time_offset_sec;
		$time_next_hh24_mm = TimeManager::convert_seconds_to_hh24_mm($time_next_sec); // $time_init_hh24_mm
		if(TimeManager::is_not_valid_time_format_hh24_mm($time_next_hh24_mm)) {
			echo "TimeManager::is_not_valid_time_format_hh24_mm(\$time_next_hh24_mm)<br/>";
			return;
		}

		$child_action_item =  
		new ActionItemTitleNTimeHHMM(
			// $action_name=""
			"Call Meeting to adjourn & Group photos"
			// $time_str_hhmm_init=""
			, $time_init_hh24_mm
			// $time_str_hhmm_specific="" / "21:40"
			, $time_next_hh24_mm
		);
		$root_action_obj->add_child($child_action_item);

		// 11-1. Sublist
		$child_action_sub_list = 
		new ActionCollection(
			// $action_name=""
			"Call Meeting to adjourn & Group photos - Sublist"
			// $action_collection_type=1
			, ActionCollection::$ACTION_COLLECTION_TYPE_LIST
		);
		$child_action_item->add_child($child_action_sub_list);

		// 11-1-1. Club News ( What's new? )
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Closing remarks ( < 2min )"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);

		// 11-1-2. Announcement & Role Sign-up for future meeting
		$child_action_sub_list_child_action_item =  
		new ActionItem(
			// $action_name="New Item"
			"Group Photo"
			// $action_item_type=1
			, ActionItem::$TITLE_ONLY_ADDABLE
		);
		$child_action_sub_list->add_child($child_action_sub_list_child_action_item);
		// 21:40

		// Refresh coordinate
		$root_action_obj->set_coordinate();

		return $root_action_obj;
	}




































}

?>
