var stone = [];
var mark = [];
var height = [0, 0, 0, 0, 0, 0, 0];
var moves = [];
var solutions = [];
var board = [
[0, 0, 0, 0, 0, 0],
  [0, 0, 0, 0, 0, 0],
  [0, 0, 0, 0, 0, 0],
  [0, 0, 0, 0, 0, 0],
  [0, 0, 0, 0, 0, 0],
  [0, 0, 0, 0, 0, 0],
  [0, 0, 0, 0, 0, 0]];
var won = false;
var player_1_auto = false;
var player_2_auto = false;


function auto_play() {
  if(moves.length % 2) return player_2_auto;
  else return player_1_auto;
}

function canplay(col) {
  if(auto_play()) return false;
  return !won && height[col] < 6;
}

function play(col, dont_compute) {
  board[col][height[col]] = 1 + moves.length % 2;
  drop(stone[moves.length], col, height[col]);
  height[col]++;
  moves.push(col);
  win();
  window.history.replaceState(null, null, get_pos_param());
  if(dont_compute) return;
  print_solution();
}

function click_handler(col) {
  return function () {
    if(canplay(col)) {
      play(col);
      gtag('event', 'game_play');
    }
  };
}

function print_next_player() {
  if(won) {
    $("#player_1").removeClass("active").addClass("inactive");
    $("#player_2").removeClass("active").addClass("inactive");
  }
  else if(moves.length % 2 == 0) {
    $("#player_1").removeClass("inactive").addClass("active");
    $("#player_2").removeClass("active").addClass("inactive");
  }
  else {
    $("#player_1").removeClass("active").addClass("inactive");
    $("#player_2").removeClass("inactive").addClass("active");
  }
}

function drop(img, x, y) {
  img.css("left", (x*100/7)+"%");
  img.css("top", (-100/7)+"%");
  img.show();
  img.animate({top: ((5-y)*100/7)+"%"}, 400, "linear", show_win);
}

function mark_win(x, y, dx, dy) {
  for (var i = 0; i < 4; i++) {
    mark[i].css("left", ((x + i * dx)*100/7) + '%');
    mark[i].css("top", ((5 - (y + i * dy))*100/7) + '%');
  }
}

function show_win() {
  if(won)
    for(var i = 0; i < 4; i++) {
      mark[i].show();
    }
}


function win() {
  var x = moves[moves.length - 1];
  var y = height[x] - 1;

  if (y >= 3 && board[x][y - 3] == board[x][y] && board[x][y - 2] == board[x][y] && board[x][y - 1] == board[x][y]) {
    mark_win(x, y, 0, -1);
    won = true;
  }
  for (var dy = -1; dy <= 1; dy++) {
    var nb = 0;
    for (var dx = 1; x + dx < 7 && y + dx * dy < 6 && y + dx * dy >= 0; dx++)
      if (board[x + dx][y + dx * dy] == board[x][y]) nb++;
      else break;
    for (var dx = -1; x + dx >= 0 && y + dx * dy < 6 && y + dx * dy >= 0; dx--)
      if (board[x + dx][y + dx * dy] == board[x][y]) nb++;
      else break;
    if (nb >= 3) {
      mark_win(x + dx + 1, y + (dx + 1) * dy, 1, dy);
      won = true;
    }
  }
}


function get_pos() {
  var pos = "";
  for (var i = 0; i < moves.length; i++) pos = pos + (moves[i] + 1);
  return pos;
}

function get_pos_param() {
  var pos=get_pos();
  if(pos == "") return location.pathname;
  else return "?pos=" + pos;
}

function  print_solution() {
  print_next_player();                          

  if(won) {
    var player = moves.length%2 == 0 ? "Yellow" : "Red";
    var message = "[1] won".replace("[1]", player);
    $("#solution_header").text(message);
    $(".solution").text("").css("background-image","none");
    gtag('event', 'game_end');
    return;
  }
  if(moves.length == 42) {
    var message = "Draw game";
    $("#solution_header").text(message);
    $(".solution").text("").css("background-image","none");
    gtag('event', 'game_end');
    return;
  }

  var json = solutions[moves.length];
  if(json === undefined || json.pos !== get_pos()) { //TODO check != ??
    compute_solution();
    return;
  }

  if(auto_play()) {
    var best = -100;
    var possible = [];
    for (var i = 0; i < 7; i++) 
      if(json.score[i] != 100 && json.score[i] > best) {
        best = json.score[i];
        possible = [i];
      }
      else if(json.score[i] == best) possible.push(i);

    var col = possible[Math.floor((Math.random() * possible.length))];
    play(col);
  }

  else {

    var best = -100;
    for (var i = 0; i < 7; i++)
      if(json.score[i] != 100 && json.score[i] > best) best = json.score[i];

    var nb_best = 0;
    for (var i = 0; i < 7; i++)
      if(best == 0 && json.score[i] == 0) nb_best++;
      else if(best > 0 && json.score[i] > 0 && json.score[i] != 100) nb_best++;



    for (var i = 0; i < 7; i++) {
      if(json.score[i] == 100) {
        $("#sol" + i).text("").css("background-image","none");
      }
      else {
        $("#sol" + i).text(json.score[i]);
        if(json.score[i] > 0) {
          if(json.score[i] == best) $("#sol" + i).css("background-image","url(/up_star.png)");
          else $("#sol" + i).css("background-image","url(/up.png)");
        }
        else if(json.score[i] < 0) {
          if(json.score[i] == best) $("#sol" + i).css("background-image","url(/down_star.png)");
          else $("#sol" + i).css("background-image","url(/down.png)");
        }
        else {
          if(json.score[i] == best) $("#sol" + i).css("background-image","url(/neutral_star.png)");
          else $("#sol" + i).css("background-image","url(/neutral.png)");
        }
      }
    }

    var message;
    if(best > 0) {
      var nb_moves = Math.floor((45 - moves.length)/2) - best;
      if(nb_moves > 1) message = "[1] can win in [2] moves".replace("[2]", nb_moves);
      else message = "[1] can win in 1 move";
    }
    else if(best < 0) {
      var nb_moves = Math.floor((44 - moves.length)/2) + best;
      if(nb_moves > 1) message = "[1] loses in [2] moves".replace("[2]", nb_moves);
      else message = "[1] loses in 1 move";
    }
    else {
      message = "[1] can draw";
    }

    var player = moves.length%2 == 0 ? "Red" : "Yellow";
    $("#solution_header").text(message.replace("[1]", player));

  }
}

function compute_solution() {  
  $("#solution_header").text("computing solution... ");
  $(".solution").text("").css("background-image","none");
  $.getJSON("/solve", {pos: get_pos()}, function(json) {
      solutions[json.pos.length] = json;
      print_solution();
      });
}

function back(event) {
  if (moves.length > 0) {
    var col = moves.pop();
    height[col]--;
    stone[moves.length].hide();
    board[col][height[col]] = 0;
    if (won) {
      won = false;
      $(".win").hide();
    }
    if(auto_play()) 
      back();
    else {
      print_solution();
      if(event == undefined) history.replaceState(null, null, get_pos_param());
      else history.pushState(null, null, get_pos_param());
    }
  }
  else if(auto_play()) {
    print_solution();
  }
  gtag('event', 'game_back');
}

function query_param(key) {
    var match = location.search.match(new RegExp("[?&]"+key+"=([^&]+)(&|$)"));
    return (match == null) ? "" : match[1];
}

function reset() {
  if(moves.length > 0) {
    moves = [];
    for(var i =0; i < 7; i++) {
      height[i] = 0;
      for(var j =0; j < 6; j++) board[i][j] = 0;
    }
    won = false;
    $(".win").hide();
    $(".player1").hide();
    $(".player2").hide();
    print_solution();
    history.replaceState(null, null, get_pos_param());
  }
  gtag('event', 'game_reset');
}

function init() {
  for (var x = 0; x < 7; x++) {
    for (var y = 0; y < 6; y++) {
      var v = $("<div/>");
      v.addClass("board");
      v.css("left", (x*100/7)+'%');
      v.css("top",  ((5-y)*100/7)+'%');
      v.click(click_handler(x));
      v.appendTo("#board");
    }
  } 

  for (i = 0; i < 42; i++) {
    var v = $("<div/>");
    if (i % 2 === 0) v.addClass("player1");
    else v.addClass("player2");
    v.hide();
    v.appendTo("#board");
    stone[i] = v;
  }
  for (i = 0; i < 4; i++) {
    var v = $("<div/>");
    v.addClass("win");
    v.hide();
    v.appendTo("#board");
    mark[i] = v;
  }

  var pos = query_param("pos");
  history.replaceState(null, null, "");
  history.pushState(null, null, "");
  $(window).on("popstate", back);

  while(pos.length > 0) {
    var c = pos.charCodeAt(0) - "1".charCodeAt(0);
    if(c >= 0 && c <= 6 && canplay(c)) play(c, true);
    else break;
    pos = pos.substr(1);
  }
  print_solution();
}

function about() {
  if($("#about_div").is(":visible")) gtag('event', 'about_hide');
  else gtag('event', 'about_show');
  $("#about_div").fadeToggle();
}

function toggle_solution() {
  if($("#hide_solution_div").is(":visible")) gtag('event', 'solution_show');
  else gtag('event', 'solution_hide');
  $("#hide_solution_div").fadeToggle();
}

function toggle_player1() {
  if(player_1_auto) {
    $("#player_1").text("manual");
    player_1_auto = false;
    gtag('event', 'autoplay_off');
  }
  else {
    $("#player_1").text("auto");
    $("#player_2").text("manual");
    player_1_auto = true;
    player_2_auto = false;                                      
    print_solution();
    gtag('event', 'autoplay_on');
  }
}

function toggle_player2() {
  if(player_2_auto) {
    $("#player_2").text("manual");
    player_2_auto = false;
    gtag('event', 'autoplay_off');
  }
  else {
    $("#player_2").text("auto");
    $("#player_1").text("manual");
    player_2_auto = true;
    player_1_auto = false;
    print_solution();
    gtag('event', 'autoplay_on');
  }
}
