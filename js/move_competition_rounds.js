$(document).ready(
	function () {
		$('div.groupWrapper').Sortable(
			{
				accept: 'groupItem',
				helperclass: 'sortHelper',
				activeclass: 'sortableactive',
				hoverclass:	'sortablehover',
				handle: 'div.itemHeader',
				tolerance: 'pointer',
				onChange : function(ser)
				{
				},
				onStart : function()
				{
					$.iAutoscroller.start(this, document.getElementsByTagName('body'));
				},
				onStop : function()
				{
					$.iAutoscroller.stop();
                    
                    var vote_round = this.parentNode.getAttribute('id');

                    var tmp = this.getAttribute('id').split('_');
                    var c_id = tmp[0];
                    var competition_round = tmp[1];

                    // Put an animated GIF image insight of content
					$("#message").empty().html('<img src="/misc/loading.gif" />');

					// Make AJAX call
                    $("#message").load('./../../../../../updatecompetitionround/' + c_id + '/' + competition_round + '/' + vote_round );
                    $("#message").fadeOut(3000);
				}
			}
		);
	}
);
