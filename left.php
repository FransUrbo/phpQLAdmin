<?php
// navigation bar
// left.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();
require("pql.inc");
require("pql_control.inc");
require("pql_control_plugins.inc");

$_pql = new pql($USER_DN, $USER_PASS);
?>
<html>
<head>
  <!-- This script was originally developed for
       phpMyAdmin (phpmyadmin.sourceforge.net)

       See left.js in this directory for a complete
       list of authors.
  -->

  <!-- Collapsable user list scripts -->
  <script type="text/javascript" language="javascript">
    <!--
    var isDOM      = (typeof(document.getElementsByTagName) != 'undefined'
                      && typeof(document.createElement) != 'undefined')
                   ? 1 : 0;
    var isIE4      = (typeof(document.all) != 'undefined'
                      && parseInt(navigator.appVersion) >= 4)
                   ? 1 : 0;
    var isNS4      = (typeof(document.layers) != 'undefined')
                   ? 1 : 0;
    var capable    = (isDOM || isIE4 || isNS4)
                   ? 1 : 0;
    // Uggly fix for Opera and Konqueror 2.2 that are half DOM compliant
    if (capable) {
        if (typeof(window.opera) != 'undefined') {
            capable = 0;
        }
        else if (typeof(navigator.userAgent) != 'undefined') {
            var browserName = ' ' + navigator.userAgent.toLowerCase();
            if (browserName.indexOf('konqueror') > 0) {
                capable = 0;
            }
        } // end if... else if...
    } // end if
    var fontFamily = 'verdana, helvetica, arial, geneva, sans-serif';
    var fontSize   = 'small';
    var fontBig    = 'large';
    var fontSmall  = 'x-small';
    var isServer   = true;
    //-->
  </script>

  <script src="left.js" type="text/javascript" language="javascript1.2"></script>

  <base target="pqlmain">
  <style type="text/css">
    <!--
    body {  font-family: Arial, Helvetica, sans-serif; font-size: 10pt}
    //-->
  </style>
</head>
<?php
// find out if we're to run in ADVANCE/SIMPLE mode
if(isset($advanced)) {
    $checked = " CHECKED";
    $ADVANCED_MODE = 1;
    session_register("ADVANCED_MODE");
} else {
    $ADVANCED_MODE = 0;
    session_register("ADVANCED_MODE");
}
?>
<body bgcolor="#D0DCE0">
  <font color="black" class="heada">
    <?=PQL_USER; ?>: <b><?=$USER_ID?></b> |
    <a href="index.php?logout=1" target="_parent"><?=PQL_LOGOUT?></a>
  </font>

  <br><br>

  <div id="el1Parent" class="parent">
    <!-- HOME -->
    <a class="item" href="home.php">
      <font color="black" class="heada"><b><?=PQL_HOME?></b></font>
    </a>
  </div>

<?php
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

    <a class="item" href="domain_detail.php?domain=<?=$domain?>" onClick="if (capable) {expandBase('el<?=$j?>', false)}">
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
	  $users = pql_get_user($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain);
	  
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
		  $cn = pql_get_userattribute($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user, PQL_LDAP_ATTR_CN);
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
	if($j > $max) {
	    $max = $j;
	}
    } // end foreach ($domains)
} // end if(is_array($domains))

// TODO: How do we know if the user is allowed to add domains?
//       In the domain description we don't have that info...
//if(PQL_LDAP_CONTROL_USE && ($USER_BASE == 'everything')) {
if(PQL_LDAP_CONTROL_USE) {
	$j = $max + 1;
?>

  <!-- ========================== -->

  <!-- Server Control -->
  <div id="el<?=$j?>Parent" class="parent">
    <br>
    <a class="item" href="control.php">
      <font color="black" class="heada"><b>Control</b></font>
    </a>
  </div>

<?php
	$j++;

	$hosts = pql_control_get_hosts($_pql->ldap_linkid, PQL_LDAP_CONTROL_BASEDN);
	if(!is_array($hosts)) {
?>
  <div id="el<?=$j?>Parent" class="parent">
    <img src="images/navarrow.png" width="9" height="9" border="0">
    <font color="black" class="heada">no LDAP control hosts defined</font>
  </div>

<?php
	} else {
		// for each host, get LDAP/Control plugins
		foreach($hosts as $host) {
?>
  <!-- start server control host: <?=$host?> -->
  <div id="el<?=$j?>Parent" class="parent">
    <a class="item" href="control_detail.php?host=<?=$host?>" onClick="if (capable) {expandBase('el<?=$j?>', true); return false;}">
      <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?=$j?>Img">
    </a>

    <a class="item" href="control_detail.php?host=<?=$host?>" onClick="if (capable) {expandBase('el<?=$j?>', false)}">
      <font color="black" class="heada"><?=$host?></font>
    </a>
  </div>
  <!-- end server control host -->

<?php
			$control_cats = pql_control_plugin_get_cats();
			if(!is_array($control_cats)) {
?>
  <!-- start server control attribute -->
  <div id="el<?=$j?>Child" class="child">
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;
      <img src="images/navarrow.png" width="9" height="9" border="0">
      <font color="black" class="heada">no plugins defined</font>
    </nobr>
  </div>
  <!-- end server control attribute -->

<?php
			} else {
				asort($control_cats);
?>
  <!-- start server control attribute: <?=$cat?> -->
  <div id="el<?=$j?>Child" class="child">
<?php

				foreach($control_cats as $cat){
?>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;
      <a href="control_cat.php?host=<?=$host?>&cat=<?=urlencode($cat)?>"><img src="images/navarrow.png" width="9" height="9" border="0"></a>&nbsp;
      <a class="item" href="control_cat.php?host=<?=$host?>&cat=<?=urlencode($cat)?>"><?=$cat?></a>
    </nobr>

    <br>

<?php
				} // end foreach controls
?>
  </div>
  <!-- end server control attribute -->
<?php
			} // end if is_array($control_cats)

			$j++;
		} // end foreach host
	} // end if is_array($hosts)
} // end if PQL_LDAP_CONTROL_USE
?>

  <form method=post action="left.php" target="pqlnav">
    <input type="checkbox" name="advanced" accesskey="a" onChange="this.form.submit(); parent.frames.pqlmain.location.reload();"<?=$checked?>><u>A</u>dvanced mode
  </form>

  <!-- Arrange collapsible/expandable db list at startup -->
  <script type="text/javascript" language="javascript1.2">
    <!--
    if (isNS4) {
      firstEl  = 'el1Parent';
      firstInd = nsGetIndex(firstEl);
      nsShowAll();
      nsArrangeList();
    }
    expandedDb = '';
    //-->
  </script>
</body>
</html>
