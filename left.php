<?php
// navigation bar
// left.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();

require("./include/pql.inc");
require("./include/pql_control.inc");
require("./include/pql_control_plugins.inc");

require("./left-head.html");

$_pql = new pql($USER_HOST_USR, $USER_DN, $USER_PASS, false, 0);
// TODO: If we're choosing another LDAP server, how do we login?
if($_pql->ldap_error) {
//    session_unregister("USER_ID");
//    session_unregister("USER_PASS");
//    session_unregister("USER_DN");

//    $_pql = new pql($USER_HOST_USR, $USER_DN, $USER_PASS);
    die("$_pql->ldap_error<a href=\"javascript:window.open('index.php')\" target=_top>Login again</a>");
}

// find out if we're to run in ADVANCE/SIMPLE mode
if($advanced == 1) {
    $checked  = " CHECKED";
    $ADVANCED_MODE = 1;

    session_register("ADVANCED_MODE");
} else {
    $checked  = "";
    $ADVANCED_MODE = 0;

    session_register("ADVANCED_MODE");
}
?>
  <font color="black" class="heada">
    <?=PQL_USER; ?>: <b><?=$USER_ID?></b> |
    <a href="index.php?logout=1" target="_parent"><?=PQL_LOGOUT?></a>
  </font>
  <form method=post action="index2.php" target="_top">
    <input type="checkbox" name="advanced" accesskey="a" onChange="this.form.submit()"<?=$checked?>><u>A</u>dvanced mode
  </form>

  <!-- HOME -->
  <div id="el1Parent" class="parent">
    <a class="item" href="home.php">
      <font color="black" class="heada"><b>Home</b></font>
    </a>
  </div>

<?php
if($ADVANCED_MODE) {
?>
  <font color="black" class="heada">
    Host: <font color="black" size=-4><b><?=$USER_HOST_USR?></b></font>
  </font>
<?php
}

// Get ALL domains we have access.
// 'description: administrator=USER_DN' in the domain object
$domains = pql_get_domain_value($_pql->ldap_linkid, '*', 'administrator', "=" . $USER_DN);
if(is_array($domains)){
    asort($domains);
} else {
    // if no domain defined, report it
?>
  <!-- start domain parent -->
  <div id="el0000Parent" class="parent">
    <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el0000Img">
    <font color="black" class="heada">no domains</font></a>
  </div>
  <!-- end domain parent -->

<?php
}

if(is_array($domains)){
    foreach($domains as $key => $domain) {
	$j = $key + 2;
?>
  <!-- start domain parent -->
  <div id="el<?=$j?>Parent" class="parent">
    <a class="item" href="domain_detail.php?domain=<?=$domain?>" onClick="if (capable) {expandBase('el<?=$j?>', true); return false;}">
      <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?=$j?>Img">
    </a>

    <a class="item" href="domain_detail.php?domain=<?=$domain?>">
      <font color="black" class="heada"><?=$domain?></font>
    </a>
  </div>
  <!-- end domain parent -->

  <!-- start domain children -->
  <div id="el<?=$j?>Child" class="child">
<?php
      // iterate trough all users
      if(PQL_SHOW_USERS) {
	  // Zero out the variables, othervise we won't get users in
	  // specified domain, but also in the PREVIOUS domain shown!
	  $users = ""; $cns = "";

	  // Get all users in the domain
	  $users = pql_get_user($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain);
	  
	  if(!is_array($users)){
	      // no user available for this domain
?>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;
      <img src="images/mail_small.png" border="0" alt="no user defined">&nbsp;
      <a href="user_add.php?domain=<?php echo $domain;?>">no user</a>
    </nobr>

    <br>

<?php
          } else {
?>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;
      <a href="user_add.php?domain=<?php echo $domain;?>">Add a user</a>
    </nobr>

    <br>

<?php
              foreach ($users as $user) {
		  $cn = pql_get_userattribute($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain, $user, PQL_LDAP_ATTR_CN);
		  $cns[$user] = $cn[0];
	      }
	      asort($cns);
    
              foreach($cns as $user => $cn){
?>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;
      <a href="user_detail.php?domain=<?php echo $domain;?>&user=<?php echo urlencode($user);?>"><img src="images/mail_small.png" border="0" alt="<?=$cn?>"></a>&nbsp;
      <a class="item" href="user_detail.php?domain=<?php echo $domain;?>&user=<?php echo urlencode($user);?>"><?php echo $cn;?></a>
    </nobr>

    <br>

<?php
              }
          }
      }
?>
  </div>
  <!-- end domain children -->

<?php
    } // end foreach ($domains)
} // end if(is_array($domains))

require("./left-trailer.html");
?>
