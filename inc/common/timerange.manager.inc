<?php

class TimeRangeManager{

	public static $HTML_TAG="HTML_TAG";
	public static $TITLE="TITLE";
	public static $ID="ID";

	private $start_time;
	private $end_time;
	private $title;
	private $id;
	public function TimeRangeManager($title="", $start_time="", $end_time=""){

		if(empty($title)) {
			$title = "time_range";
		}

		$this->title = $title;

		// 입력하지 않았을 때는 시작은 오늘, 종료일은 일주일 전으로 합니다.
		$QUIZ_DATE_START = $param->get_param_string($param->QUIZ_DATE_START, TimeManager::get_days_ago(5));
		if(!empty($QUIZ_DATE_START)) {
			$result->{$param->QUIZ_DATE_START} = $QUIZ_DATE_START;
		}		
		$QUIZ_DATE_END = $param->get_param_string($param->QUIZ_DATE_END, TimeManager::get_now_YYYYMMDD());
		if(!empty($QUIZ_DATE_END)) {
			$result->{$param->QUIZ_DATE_END} = $QUIZ_DATE_END;
		}
		$QUIZ_DATE_END_DAY_AFTER = TimeManager::get_days_after(1, $QUIZ_DATE_END); // $QUIZ_DATE_END_DAY_AFTER
		if(!empty($QUIZ_DATE_END_DAY_AFTER)) {
			$result->{$param->QUIZ_DATE_END_DAY_AFTER} = $QUIZ_DATE_END_DAY_AFTER;
		}


		$this->start_time = $start_time;
		$this->end_time = $end_time;

		// set unique id
		$time_range_id = Checker::get_hash_key($title);
		$this->set_id($time_range_id);

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


	
	// @ Usage : TimeRangeManager::get_combobox();
	// @ Desc : html tag, id 및 관련 정보를 돌려줍니다.
	public function get_button() {

		$target_option_arr = $this->option_arr;

		$style_width = "";
		if(!empty($width)) {
			$style_width = "width:$width;";
		}

		$button_std = new stdClass();
		$button_std->{TimeRangeManager::$ID} = $this->get_id();

		$button_title = $this->get_title();
		$button_id = $this->get_id();

		$button_tag = "";
		$button_tag = "" 
		. "<div class=\"col-sm-1\" style=\"padding-left:0px;\">"
				. "<button id=\"$button_id\" type=\"button\" class=\"btn btn-primary\">"
					// . "<span class=\"glyphicon glyphicon-eye-open\" aria-hidden=\"true\" style=\"padding-right:10px;\"></span>"
					. "<strong>Show</strong>"
				. "</button>"
		. "</div>"
		;

		$button_std->{TimeRangeManager::$HTML_TAG} = $button_tag;		

		return $button_std;

	}

}

?>
