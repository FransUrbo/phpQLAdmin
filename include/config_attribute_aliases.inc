<?php
// In '[domain]->Branch Owner Details' there's a lot of attributes
// with very cryptic names. For example 'l' means 'localityName', but
// that's still cryptic. In plain english, it simply means 'city'...
//
// This file is intended to make simple translations from the short
// attribute name to more 'simple to read' text.
// It is not intended to be used to translate ALL attributes (some
// actually have resonable text), but for those those that are VERY
// cryptic...
//
// It is (currently) only used in the include/attrib.outlook.inc:attribute_print_form()
// function, but if/when I feel the need, this file will expand and
// used else where as well.

$ATTRIBUTE_ALIAS = array('l'	=> $LANG->_('City'),
			 'c'	=> $LANG->_('Country'),
			 'o'	=> $LANG->_('Organization'),
			 'st'	=> $LANG->_('State'));
?>

