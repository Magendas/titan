<?php

class ArrayManager{

	// @ Usage : ArrayManager::get_queue_group($target_array=null, $element_cnt_per_queue=-1)
	public static function get_queue_group($target_array=null, $element_cnt_per_queue=-1){

		if($element_cnt_per_queue < 2) {
			$element_cnt_per_queue = 50;
		}

		if(is_null($target_array) || empty($target_array)) {
			return null;
		}

		$arr_queue_group = array();
		$arr_queue = array();
		for($idx = 0;$idx < count($target_array); $idx++) {

			if(0 < $idx && ($idx%$element_cnt_per_queue) == 0) {

				// move to next queue
				array_push($arr_queue_group, $arr_queue);
				$arr_queue = array();

			}
			
			$element = $target_array[$idx];
			array_push($arr_queue, $element);

			if($idx == (count($target_array) - 1)) {

				// reach the last index
				array_push($arr_queue_group, $arr_queue);

			} // end if


		} // end for

		return $arr_queue_group;
	}

}

?>
