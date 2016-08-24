<?php

class PageManager{

	private static $CURSOR_PAGE_NUM_DEFAULT = 1;
	private static $ROW_CNT_DEFAULT = 5;
	private static $PAGE_RANGE_DEFAULT = 5;
	
	// @ Usage : PageManager::get_pagenation($total_row_cnt, $cursor_page_num);
	public static function get_pagenation($total_row_cnt=-1, $cursor_page_num=-1, $row_cnt_per_page=-1, $page_cnt_on_pagination=-1) {

		if(Checker::is_not_unsigned_number($total_row_cnt)) {
			return;
		}

		if(Checker::is_not_unsigned_number($cursor_page_num)) {
			$cursor_page_num=PageManager::$CURSOR_PAGE_NUM_DEFAULT;
		}

		if(Checker::is_not_unsigned_number($row_cnt_per_page)) {
			$row_cnt_per_page=PageManager::$ROW_CNT_DEFAULT;
		}

		if(Checker::is_not_unsigned_number($page_cnt_on_pagination)) {
			$page_cnt_on_pagination=PageManager::$PAGE_RANGE_DEFAULT;
		}

		$param = new ParamManager();

		$pagenation_std = new stdClass();
		$pagenation_std->{$param->PAGE_NUM} = $cursor_page_num;
		$pagenation_std->{$param->ROW_CNT} = $row_cnt_per_page;
		$pagenation_std->{$param->PAGE_RANGE} = $page_cnt_on_pagination;

		// 전체 row는 100개
		// $total_row_cnt=100;
		// 페이지당 10개의 row를 보여줌
		// $row_cnt_per_page=10;
		// 전체 페이지는 10 페이지
		// $total_page_cnt=10;

		// 페이지네이션에서 보여주는 페이지 갯수는 10개
		// $page_cnt_on_pagination=10;
		// 현재 보여주는 페이지의 번호
		// $cursor_page_num=7;

		// 화면에서 보여주는 시작 페이지 번호
		// $page_num_begin_on_view = 1;
		// 화면에서 보여주는 끝 페이지 번호
		// $page_num_end_on_view = 10;

		// 이전 페이지네이션을 보여주는 페이지 번호 / 여기서는 최초 1페이지이므로 이전 페이지네이션이 없습니다.
		// $page_num_jump_to_prev=0;
		// 다음 페이지네이션을 보여주는 페이지 번호
		// $page_num_jump_to_next=11;
		// 마지막 페이지 번호 - 마지막 페이지네이션이 아닌 경우는 화면에 표시되지 않습니다.
		// $page_num_last=10;

		// 화면에서 보여주는 형태
		// <<(0)prev | 1 | 2 | 3 | 4 | 5 | 6 | You're here - (7) | 8 | 9 | 10 | next(11)>>

		$page_num_begin_on_view = $page_cnt_on_pagination * floor(($cursor_page_num - 1) / $page_cnt_on_pagination) + 1;
		$page_num_end_on_view = $page_num_begin_on_view + $page_cnt_on_pagination;

		$total_page_cnt = ceil($total_row_cnt/$row_cnt_per_page);
		$page_num_jump_to_prev = $page_num_begin_on_view - 1;
		if($total_page_cnt < $page_num_end_on_view) {
			$page_num_jump_to_next = -1;
			$page_num_end_on_view = $total_page_cnt + 1;
		} else {
			$page_num_jump_to_next = $page_num_end_on_view;									
		}
		$page_num_last = $total_page_cnt;		


		
		$pagenation_std->{$param->TOTAL_ROW_CNT} = $total_row_cnt;
		$pagenation_std->{$param->CURSOR_PAGE_NUM} = $cursor_page_num;
		$pagenation_std->{$param->ROW_CNT_PER_PAGE} = $row_cnt_per_page;
		$pagenation_std->{$param->PAGE_CNT_ON_PAGINATION} = $page_cnt_on_pagination;

		$pagenation_std->{$param->TOTAL_PAGE_CNT} = $total_page_cnt;
		$pagenation_std->{$param->PAGE_NUM_BEGIN_ON_VIEW} = $page_num_begin_on_view;
		$pagenation_std->{$param->PAGE_NUM_END_ON_VIEW} = $page_num_end_on_view;

		$pagenation_std->{$param->PAGE_NUM_JUMP_TO_PREV} = $page_num_jump_to_prev;
		$pagenation_std->{$param->PAGE_NUM_JUMP_TO_NEXT} = $page_num_jump_to_next;
		$pagenation_std->{$param->PAGE_NUM_LAST} = $page_num_last;

		// pagenation bootstrap html tag
		$html_bootstrap_tag = ""
		. "<nav style=\"text-align:center;\"><ul class=\"pagination\">"
		;

		// pagenation / Arrow / Prev
		if(0 < $pagenation_std->PAGE_NUM_JUMP_TO_PREV) {
			$html_bootstrap_tag .= ""
			. "<li page=\"$pagenation_std->PAGE_NUM_JUMP_TO_PREV\"><a href=\"#\" aria-label=\"Previous\"><span aria-hidden=\"true\">&laquo;</span></a></li>";	
		}

		for($idx = $pagenation_std->PAGE_NUM_BEGIN_ON_VIEW; $idx < $pagenation_std->PAGE_NUM_END_ON_VIEW; $idx++) {
			if($cursor_page_num == $idx) {
				$html_bootstrap_tag .= ""
				. "<li class=\"active\" page=\"$idx\"><a href=\"#\">$idx</a></li>"
				;
			} else {
				$html_bootstrap_tag .= ""
				. "<li page=\"$idx\"><a href=\"#\">$idx</a></li>"
				;	
			}
		}

		// pagenation / Arrow / Next
		if(0 < $pagenation_std->PAGE_NUM_JUMP_TO_NEXT) {
			$html_bootstrap_tag .= ""
			. "<li page=\"$pagenation_std->PAGE_NUM_JUMP_TO_NEXT\"><a href=\"#\" aria-label=\"Next\"><span aria-hidden=\"true\">&raquo;</span></a></li>";	
		}
		$html_bootstrap_tag .= ""
		. "</ul></nav>";

		$pagenation_std->{$param->BOOTSTRAP_TAG} = $html_bootstrap_tag;

		return $pagenation_std;
	}
}

?>
