<?php
// Fix a 'object class violation' by adding missing MUST attributes
// $Id: user_add_missing_attributes.php,v 2.7 2005-09-16 06:08:44 turbo Exp $
//
// This file is INCLUDED from pql.inc:pql_replace_userattribute()
// and should NOT be called directly!
//
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
?>

  <span class="title1"><?=$LANG->_('Object class violation')?></span>
  <br><br>

  Sorry, but the request you tried to perform (modify attribute <b><?=$MISSING['attrib']?></b>)
  did not work - the object you tried to modify is missing an object class.
  <br><br>
  To be able to fullfill the query, you will have to add the objectclass 
  <b><?=$MISSING['objectclass']?></b> to the object.
  <br>
  This objectclass however require the following attributes (which isn't in the object either):
  <br><br>
  <center>
<?php
for($i=0; $i < count($MISSING['attributes']); $i++) {
	echo "<b>".$MISSING['attributes'][$i]."</b>";
	if(!$MISSING['attributes'][$i+2] and $MISSING['attributes'][$i+1])
		echo " and ";
	elseif($MISSING['attributes'][$i+1])
		echo ", ";
}
?>
  </center>
  <br>
  There is (currently) no way that phpQLAdmin will be able to let you input these attributes
  and try again. The only way I can think of to get around this is to disable schema checking
  in the LDAP server, but I do not recomend this.

  If you are the administrator of this phpQLAdmin installation, and you just happen to know
  a little PHP, then I'd appreciate some help with this :)

  The offending file is 'user_add_missing_attributes.php'...
<?php
pql_flush();

/*
 * Local variables:
 * mode: php
 * tab-width: 4
 * End:
 */
?>
