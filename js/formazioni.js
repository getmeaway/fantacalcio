function change_round(competition, c_id, base_url){
	var roundCompetition = document.getElementById("round_" + c_id);
	var url = base_url + 'formazioni/view/' + competition + '/' + roundCompetition.value;
	location.href = url;
}