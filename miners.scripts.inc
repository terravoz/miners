<?php


/**
 * Implementation of hook_voipscript_get_script_names()
 */
function miners_voipscript_get_script_names() {
  $script_names[] = 'miners_main_menu_script';
  return $script_names;
}

/**
 * Implementation of hook_voipscript_load_script()
 */
function miners_voipscript_load_script($script_name, $options=NULL) {

  $script = NULL;
  switch($script_name) {
    default:
      break;

	case 'miners_main_menu_script':
      $script = new VoipScript('miners_main_menu_script');
      voipvoice_set_current_voice('miners');

      $script->addLabel('options_menu');

      $p_options_menu = v("You have called Miners Matters, an interactive hotline for miners working in the United States. "
	  . "To hear information about your rights as a miner and workplace safety, press 1. "
. "To hear up-to-date information about a particular jobsite, mine, or safety product, press 2. "
. "To be directed to a miners advocacy group in your region, press 3. "
. "To make a complaint directly to OSHA (the Occupational Safety and Hazards Administration), press 4. "
. "To hear about the creators of Miners Matters, press 5. ");

      $input_options = array(
        '1' => 'miner_rights',
        '2' => 'jobsite_info',
        '3' => 'advocacy',
        '4' => 'osha',
        '5' => 'credits',
		'#' => 'hang_up',
		'i' => 'invalid_option',
        't' => 'invalid_option',
      );

      $p_invalid_msg = v('Invalid option selected');
      $script->addRunIvrMenu($p_options_menu, $input_options, $p_invalid_msg);
      $script->addGoto('%ivr_option_selected');

      $script->addLabel('miner_rights');
      $script->addGosub('miner_rights_script', $options);
      $script->addGoto('options_menu');

      $script->addLabel('jobsite_info');
      $script->addGosub('jobsite_info_script', $options);
      $script->addGoto('options_menu');
	  
	  $script->addLabel('advocacy');
      $script->addGosub('advocacy_script', $options);
      $script->addGoto('options_menu');
	  
	  $script->addLabel('osha');
      $script->addGosub('osha_script', $options);
      $script->addGoto('options_menu');
	  
	  $script->addLabel('credits');
      $script->addGosub('credits_script', $options);
      $script->addGoto('options_menu');

      $script->addLabel('invalid_option');
      $script->addGoto('hang_up');

      $script->addLabel('hang_up');
      $script->addSay(v('Thanks for calling Miners Matters. Goodbye now. '));

      $script->addHangup();
      break;

    case 'miner_rights_script':
      $script = new VoipScript('miner_rights_script');
      voipvoice_set_current_voice('miners');
      
      $script->addLabel('episode_directory');

      $p_options_menu = v("You are about to hear episodes about our rights as a miner. "
. "Now listen up, to go back to the beginning of an episode, press 1 at any time. To skip to the next episode, press 2 at any time. "
. "To return to the main menu, press the # at any time. "
. "Ok, so. "
. "To learn about reporting workplace hazards, rights, and protections as a miner, press 1. "
. "To learn about your health protection rights, press 2. "
. "To learn about safety training in the workplace, press 3. "
. "To learn about safety issues and rights related to black lung disease, press 4. "
. "To hang up, press * at any time. ");

      $input_options = array(
        '1' => 'coming_soon',
        '2' => 'coming_soon',
		'3' => 'coming_soon',
        '4' => 'coming_soon',
        '*' => 'hangup',
        '#' => 'go_back',
        'i' => 'invalid_option',
        't' => 'invalid_option'
      );
      $p_invalid_msg = v('Invalid option selected');
      $script->addRunIvrMenu($p_options_menu, $input_options, $p_invalid_msg);
      $script->addGoto('%ivr_option_selected');

      $script->addLabel('workplace_hazards');
	  miners_play_story('workplace_hazards', $script);
      $script->addGoto('episode_directory');

      /*$script->addLabel('wages_episode');
	  miners_play_story('wages_episode', $script);
      $script->addGoto('episode_directory');*/
	  
	  $script->addLabel('coming_soon');
	  $script->addSay(v("This feature is coming soon. "));
	  $script->addGoto('episode_directory');
	  
      $script->addLabel('invalid_option');
      $script->addGoto('go_back');

      $script->addLabel('go_back');
      $script->addReturn();
	  
	  $script->addLabel('hangup');
	  $script->addSay(v('Thanks for calling Miners Matters. Goodbye now. '));
      $script->addHangup();
      break;

    case 'jobsite_info_script':
      $script = new VoipScript('jobsite_info_script');
      voipvoice_set_current_voice('miners');
      if(isset($options['label'])) {
	     $script->addGoto($options['label']);
	  }
	  
      $script->addLabel('jobsite_info_menu');
	  $p_options_menu = v("You have reached the interactive section of Miners Matters. "
. "To learn more about the safety and incidents of a particular mine or jobsite, press 1. "
. "To learn more about a contractor whether their license is in good standing, revoked, or other information, press 2. "
. "To return to the main menu, press the # at any time. ");

      $input_options = array(
        '1' => 'mine_safety',
        '2' => 'contractor_licence',
        '#' => 'go_back',
        'i' => 'invalid_option',
        't' => 'invalid_option'
      );
      $p_invalid_msg = v('Invalid option selected');
      $script->addRunIvrMenu($p_options_menu, $input_options, $p_invalid_msg);
      $script->addGoto('%ivr_option_selected');

      $script->addLabel('invalid_option');
      $script->addGoto('go_back');

	  /*********MINE SAFETY OPTION**********/
      $script->addLabel('mine_safety');
	  $prompt = v("Hi, caller! Interested in learning about the safety and incidents of a particular mine or jobsite? You will need to know either the 7 digit ID that refers to a particular mine or jobsite or the zipcode of that mine. When you have got either of these numbers in front of you, press 1. ");
	  $timeout = 300; // 5minutes
      $end_key = '';
      $num_digits = 1;
      $script->addGetInput($prompt, $num_digits, $end_key, $timeout);
	  $script->addGotoIf('mine_safety_submenu', "^%input_digits == '1'");
	  $script->addGotoIf('go_back', "^%input_digits == '#'");
      $script->addGoto('mine_safety'); //repeat

	  $script->addLabel('mine_safety_submenu');
      $prompt = v("Press 1 to search by zipcode. Press 2 to search by the 7 digit mine ID number. ");
	  $timeout = 60; 
      $end_key = '#';
      $num_digits = 1;
      $script->addGetInput($prompt, $num_digits, $end_key, $timeout);
	  $script->addGotoIf('zip_id', "^%input_digits == '1'");
	  $script->addGotoIf('mine_id', "^%input_digits == '2'");
	  $script->addGoto('mine_safety_submenu'); //repeat
	  
	  /*SEARCH BY MINE ID*/
      $script->addLabel('mine_id');
      $prompt = v("Enter the 7 digit ID here. Press the pound sign when done. ");
	  $timeout = 60;
      $end_key = '#';
      $num_digits = 7;
      $script->addGetInput($prompt, $num_digits, $end_key, $timeout);
	  $script->addGotoIf('mine_id', "^%input_digits == 'no_input'"); //repeat
	  //$script->addGotoIf('mine_id_read', "^strlen('%input_digits') == 7"); 
      
	 /* $script->addLabel('mine_id_incorrect');
	  $script->addSay(v("Mine id incorrect. "));
	  $script->addGoto('mine_id');
	  
	  $script->addLabel('mine_id_read');*/
	  $script->addSet('mine_id', "%input_digits");
	  $script->addSay(v("You have entered "));
	  $script->addSay(new VoipPrompt('%mine_id', 'woman', 'digits'));
	  $prompt = v("If this is correct, press 1. If not, press 2 to reenter the correct number. ");
	  $input_options = array(
        '1' => 'mine_id_confirmed',
        '2' => 'mine_id',
        '#' => 'go_back',
        'i' => 'invalid_option',
        't' => 'invalid_option'
      );

      $p_invalid_msg = v('Invalid option selected');
      $script->addRunIvrMenu($prompt, $input_options, $p_invalid_msg);
      $script->addGoto('%ivr_option_selected');

	  $script->addLabel('mine_id_confirmed');
	  $options['mine_id'] = '%mine_id';
	  $script->addGosub('miners_dynamic_accidents_submenu', $options);
	  $script->addGoto('go_back');
	  /*$prompt = array();
	  $prompt[] = v("Mine number ");
	  $prompt[] = new VoipPrompt('%mine_id', 'woman', 'digits');
	  $prompt[] = v(" is Shoal Creek Underground Coal Mine and located in Jefferson County, Alabama. "
. "The last accident or incident was on October 29, 2011. "
. "Information reported about this incident are as follows: At approximately 11:15 p m the elevator stopped at mid shaft. After checking the control room of the elevator the elevator showed fault. The fault was due to the magnet on the north gate of the elevator. The elevator was put back in service at approximately 1:40 a m. ");

      $prompt[] = v("To repeat this information, press 1. "
. "To learn more about the safety and incidents of another particular mine or jobsite, press 2. "
. "To learn more about a contractor whether their license is in good standing, revoked, or other information, press 3. "
. "To return to the main menu, press the # at any time. ");
 $input_options = array(
        '1' => 'mine_id_confirmed',
		'2' => 'mine_safety',
        '3' => 'contractor_licence',
        '#' => 'go_back',
        'i' => 'invalid_option',
        't' => 'invalid_option'
      );

      $p_invalid_msg = v('Invalid option selected');
      $script->addRunIvrMenu($prompt, $input_options, $p_invalid_msg);
      $script->addGoto('%ivr_option_selected');

      $script->addLabel('invalid_option');
      $script->addGoto('go_back');
	  */
	  
	  $script->addLabel('zip_id');
      $prompt = v("Enter the 5 digit zip code. Press the pound sign when done. ");
	  $timeout = 60;
      $end_key = '#';
      $num_digits = 5;
      $script->addGetInput($prompt, $num_digits, $end_key, $timeout);
	  $script->addGotoIf('zip_id', "^%input_digits == 'no_input'"); //repeat
	  
	  $script->addSet('zip_id', "%input_digits");
	  $script->addSay(v("You have entered "));
	  $script->addSay(new VoipPrompt('%zip_id', 'woman', 'digits'));
	  $prompt = v("If this is correct, press 1. If not, press 2 to reenter the correct number. ");
	  $input_options = array(
        '1' => 'zip_id_confirmed',
        '2' => 'zip_id',
        '#' => 'go_back',
        'i' => 'invalid_option',
        't' => 'invalid_option'
      );

      $p_invalid_msg = v('Invalid option selected');
      $script->addRunIvrMenu($prompt, $input_options, $p_invalid_msg);
      $script->addGoto('%ivr_option_selected');

	  $script->addLabel('zip_id_confirmed');
	  $options['zip_id'] = '%zip_id';
	  $script->addGosub('miners_dynamic_zip_submenu', $options);
	  $script->addGoto('go_back');
	  
	  /*********CONTRACTOR LICENCE OPTION**********/
      $script->addLabel('contractor_licence');
	  $script->addSay(v("This feature is coming soon. "));
	  $script->addGoto('jobsite_info_menu');
	  /*
	  $prompt = v("Hi, caller! I am Appalachia Annie. So the way this works is that you are gonna look for that 9 digit ID that refers to a particular manager or contractor. When you have got this number in front of you, press 1.");
	  $timeout = 300; // 5minutes
      $end_key = '';
      $num_digits = 1;
      $script->addGetInput($prompt, $num_digits, $end_key, $timeout);
	  $script->addGotoIf('contractor_id', "^%input_digits == '1'");
	  $script->addGotoIf('go_back', "^%input_digits == '#'");
      $script->addGoto('contractor_licence'); //repeat

      $script->addLabel('contractor_id');
      $prompt = v("Enter the 9 digit ID here. Press the pound sign when done. ");
	  $timeout = 60; // 5minutes
      $end_key = '#';
      $num_digits = 9;
      $script->addGetInput($prompt, $num_digits, $end_key, $timeout);
	  $script->addGotoIf('contractor_id', "^%input_digits == ''"); //repeat
	  
	  $script->addGotoIf('contractor_id_read', "^strlen('%input_digits') == 9"); 
      
	  $script->addLabel('contractor_id_incorrect');
	  $script->addSay(v("Contractor id incorrect. "));
	  $script->addGoto('contractor_id');
      
	  $script->addLabel('contractor_id_read');
	  $script->addSet('contractor_id', "%input_digits");
	  $script->addSay(v("You have entered "));
	  $script->addSay("%contractor_id");
	  $prompt = v("If this is correct, press 1. If not, press 2 to reenter the correct number. ");
	  $input_options = array(
        '1' => 'contractor_id_confirmed',
        '2' => 'contractor_id',
        '#' => 'go_back',
        'i' => 'invalid_option',
        't' => 'invalid_option'
      );

      $p_invalid_msg = v('Invalid option selected');
      $script->addRunIvrMenu($prompt, $input_options, $p_invalid_msg);
      $script->addGoto('%ivr_option_selected');
	  //Hardcoded for prototype:
	  $script->addLabel('contractor_id_confirmed');
	  $prompt = array();
	  $prompt[] = v("The name of ID-holder %contractor_id  is Henry Hobsquatch”. His license is revoked as of February 10, 2011. for violation of employee mismanagement and harassment. ");
	  $prompt[] = v("To repeat this information, press 1. "
. "To learn about the status of another contractor, press 2. "
. "To learn more about the safety of a particular mine or jobsite, press 3. "
. "To return to the main menu, press the # at any time. ");
 $input_options = array(
        '1' => 'contractor_id_confirmed',
		'2' => 'contractor_licence',
		'3' => 'mine_safety',
        '#' => 'go_back',
        'i' => 'invalid_option',
        't' => 'invalid_option'
      );

      $p_invalid_msg = v('Invalid option selected');
      $script->addRunIvrMenu($prompt, $input_options, $p_invalid_msg);
      $script->addGoto('%ivr_option_selected');
*/
	  $script->addLabel('invalid_option');
      $script->addLabel('go_back');
      $script->addReturn();
      break;
	case 'advocacy_script':
      $script = new VoipScript('advocacy_script');
	  voipvoice_set_current_voice('miners');
	  $script->addSay(v("This feature is coming soon. "));
	  $script->addReturn();
    break;	  
    case 'osha_script':
      $script = new VoipScript('osha_script');
	  voipvoice_set_current_voice('miners');
	  $script->addSay(v("This feature is coming soon. "));
	  $script->addReturn();
      break;
	  
	 case 'credits_script':
      $script = new VoipScript('credits_script');
      voipvoice_set_current_voice('miners');
      $script->addSay(v("Miners Matters was created by REV in collaboration with MITs Center for Civic Media. "
."REV is a a New York based non profit organization that furthers socially engaged art, design, and education. Find us online at w w w dot r e v dash i t dot e d u. "
."The MIT Center for Civic Media works hand in hand with diverse communities to collaboratively create, design, deploy, and assess civic media tools and practices. Find us online at civic dot mit dot edu. "));
      $script->addReturn();
      break;
	  
	 case 'miners_dynamic_accidents_submenu':
      $script = new VoipScript('miners_dynamic_accidents_submenu');
	  voipvoice_set_current_voice('miners');
      $mine_id = $options['mine_id'];
	  $with_zip = isset($options['zip_id']) ? $options['zip_id'] : NULL;
	  miners_get_accident_by_mine_id($mine_id, $script, $with_zip);
	  
	  $script->addLabel('back_to_mine_safety');
	  $options['label'] = 'mine_safety';
	  $script->addGosub('jobsite_info_script', $options);
	  
	  $script->addLabel('go_back');
	  $script->addReturn();
	  break;
	  
	  case 'miners_dynamic_zip_submenu':
      $script = new VoipScript('miners_dynamic_zip_submenu');
	  $script->addLabel('zip_submenu');
	  voipvoice_set_current_voice('miners');
      $zip_id = $options['zip_id'];
	  miners_get_mines_by_zip($zip_id, $script);
	  
	  $script->addLabel('reenter_zip');
	  $options['label'] = 'zip_id';
	  $script->addGosub('jobsite_info_script', $options);
	  $script->addGoto('go_back');
	  
	  $script->addLabel('mine_submenu');
	  $options['mine_id'] = '%mine_id';
	  $script->addGosub('miners_dynamic_accidents_submenu', $options);
	  
	  $script->addLabel('go_back');
	  $script->addReturn();
	  break;
  }

  return $script;
}

function miners_play_story($story_id, &$script) {

  switch($story_id) {
  default:
    $script->addSay("Invalid story id: $story_id. ");
    $script->addLog("Invalid story id: $story_id. ");
    return;
  break;
        case 'workplace_hazards':
          $prompt[] = v('Episode about Workplace hazards. ');
		  //$next = 'wages_episode';
		  $next = 'episode_directory';
          break;
       /* case 'wages_episode':
          $prompt[] = v('Here the system will play an episode about minimum wages. ');
		  $next = 'episode_directory';
          break;*/
  }
      $script->addLabel('play_story');
      $prompt[] = v("To repeat this episode, press 1. "
. "To go to the next episode, press 2. "
. "To hear a list of all Miners Matters episodes, press 3. "
. "To go back to the main menu, press the # key. "
. "To hang up, press *.");
	  
      $end_key = '';
      $num_digits = 1;
      $script->addGetInput($prompt, $num_digits, $end_key);
	  $script->addGotoIf($story_id, "^%input_digits == '1'");//replay story
	  $script->addGotoIf($next, "^%input_digits == '2'"); //next story
	  $script->addGotoIf('episode_directory', "^%input_digits == '3'"); //episode directory
	  $script->addGotoIf('go_back', "^%input_digits == '#'");
	  $script->addGotoIf('hangup', "^%input_digits == '*'");
      
      $script->addLabel('invalid_option');
	  $script->addSay(v("Invalid option selected"));
      $script->addGoto('play_story');
}

function miners_get_accident_by_mine_id($mine_id, &$script, $with_zip) {
  //watchdog('miners',"Mine id $mine_id");
  $mine_info = db_fetch_object(db_query("SELECT mine_name, city, state, mine_status FROM {mine_addresses} WHERE mine_id='%s'", $mine_id));
  
  if(!$mine_info) {
    $script->addSay(v("There are no mines associated with the given code. "));
    $script->addGoto('go_back');
	return;
  }
  $query = db_query("SELECT accident_dt, narrative FROM {mine_accidents} WHERE mine_id='%s' ORDER BY accident_dt DESC", $mine_id);
  $i=1;
  $default_prompt = v("To repeat this information, press 1. "
. "To skip and listen to the next incident at this mine, press 2. "
. "To learn  about the safety and incidents of another particular mine or jobsite, press 3. "
. "To return to the main menu, press the # at any time. ");
  $end_key = '';
  $num_digits = 1;

  while($accident = db_fetch_object($query)) {
    $mine_accident_id = 'mine_accident_'.$i;
	$next = 'mine_accident_'.($i+1);
	$script->addLabel($mine_accident_id);
	$prompt = array(); //reset
	if ($i == 1) {
	  //Read this only for first accident
	  $prompt[] = v("Mine number ");
	  $prompt[] = new VoipPrompt($mine_id , 'woman', 'digits');
	  if ($with_zip) {
	    $prompt[] = v("with a zip code of ");
	    $prompt[] = new VoipPrompt($with_zip , 'woman', 'digits');
	  }
	  $prompt[] = v(" is ");
	  $prompt[] = new VoipPrompt($mine_info->mine_name);
	  $prompt[] = v(" and located in ");
	  $prompt[] = new VoipPrompt($mine_info->city.", ".$mine_info->state. ". ");
	  $prompt[] = v("The status of this mine is ");
	  $prompt[] = new VoipPrompt($mine_info->mine_status);
	  $prompt[] = v("The last accident or incident was on ");
	}
	else {
	  $prompt[] = v("The previous incident took place on ");
	}
	
	$prompt[] = new VoipPrompt(date("F j, Y", $accident->accident_dt). ". ");
	if ($i == 1) {
	  $prompt[] = v("Information reported about this incident are as follows. ");
	}
	else {
	  $prompt[] = v(" and is as follows. ");
	}
	$prompt[] = new VoipPrompt(str_replace("'","",$accident->narrative).". ");

	$prompt[] = $default_prompt;
	$script->addGetInput($prompt, $num_digits, $end_key);
	$script->addGotoIf($mine_accident_id, "^%input_digits == '1'");//replay story
	$script->addGotoIf($next, "^%input_digits == '2'"); //next story
	$script->addGotoIf('back_to_mine_safety', "^%input_digits == '3'"); //mine_safety
	$script->addGotoIf('go_back', "^%input_digits == '#'");
	$script->addGotoIf('hangup', "^%input_digits == '*'");
	$i++;
  }
  
  if($i == 1) {
    //No accidents
	$script->addSay(v("There are no incidents associated with the specified mine. "));
    $script->addGoto('go_back');
	return;
  }

  $script->addLabel($next);
  $script->addSay(v("There are no additional incidents associated with this mine. "));
  $script->addGoto('go_back');
  
  /*$script->addLabel('invalid_option');
  $script->addSay(v("Invalid option selected"));
  $script->addGoto($mine_accident_id);*/
}

function miners_get_mines_by_zip($zip_id, &$script) {
  $query = db_query("SELECT mine_id, mine_name FROM {mine_addresses} WHERE zip_cd='%s' ORDER BY mine_name ASC", $zip_id);
  $i=1;
  
  $end_key = '';
  $num_digits = 99;
  while($mine = db_fetch_object($query)) {
    $mines[] = $mine;
	//For large number of options this used to fail!
	//$prompt[] = v("For ");
    //$prompt[] = new VoipPrompt($mine->mine_name." press ".$i);
	$label_id = $i+1; //because we use 1 for reenter zip option
	$mine_selection .= voipvoice_get_audio_phrase("For ", 'miners')."$mine->mine_name press $label_id. ";
	$mine_id = $mine->mine_id;
    $i++;
  }
  if($i == 1) {
    //No mines
    $script->addSay(v("There are no mines associated with the specified zip code. "));
    $script->addGoto('go_back');
	return;
  }
  if($i == 2) {
    //One mine only
	$script->addSet('mine_id', $mine_id);
	$script->addGoto('mine_submenu');
	return;
  }
  //Multiple mines
  $prompt[] = new VoipPrompt($mine_selection);
  $prompt[] = v("To repeat these selections, press the star key. ");
  //$default_prompt = voipvoice_get_audio_phrase("To repeat these selections, press the star key. ", 'miners');
  $prompt[] = v("To enter a different zip code, press 1. "
  . "To go back to the main menu, press the pound sign. ");
  
  $script->addGetInput($prompt, $num_digits, $end_key);
  $script->addGotoIf('zip_submenu', "^%input_digits == '**'");//repeat selections
  $script->addGotoIf('go_back', "^%input_digits == '#'");//repeat selections
  $script->addGotoIf('reenter_zip', "^%input_digits == '1'"); //reenter zip
  $script->addGotoIf('zip_submenu', "^%input_digits == 'no_input'");//repeat selections
  $script->addGotoIf('invalid_option', "^miners_mine_subid_invalid(%input_digits, $i)");//repeat selections
  $script->addGoto('mine_%input_digits'); //go to mine label
  
  $i=2; //1 is used for reenter zip option
  foreach($mines as $mine) {
    $mine_subid = 'mine_'.$i;
	$script->addLabel($mine_subid);
	$script->addSet('mine_id', $mine->mine_id);
	$script->addGoto('mine_submenu');
    $i++;
  }
  
  $script->addGoto('go_back');
  
  $script->addLabel('invalid_option');
  $script->addSay(v("Invalid option selected"));
  $script->addGoto('zip_submenu');
}

function miners_mine_subid_invalid($selected, $max) {
  if($selected>$max) {
    return TRUE;
  }
  return FALSE;
}
