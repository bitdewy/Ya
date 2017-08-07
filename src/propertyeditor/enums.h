/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

enum TextPropertyValidationMode
{
    // Allow for multiline editing using literal "\n".
    ValidationMultiLine,
    // Allow for HTML rich text including multiline editing using literal "\n".
    ValidationRichText,
    // Validate a stylesheet
    ValidationStyleSheet,
    // Single line mode, suppresses newlines
    ValidationSingleLine,
    // Allow only for identifier characters
    ValidationObjectName,
    // Allow only for identifier characters and colons
    ValidationObjectNameScope,
    // URL
    ValidationURL
};
