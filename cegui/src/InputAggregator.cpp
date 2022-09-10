////----------------------------------------------------------------------------//
//SemanticValue InputAggregator::getSemanticAction(Key::Scan scan_code, bool shift_down,
//    bool alt_down, bool ctrl_down) const
//{
//    std::fill(std::begin(d_keyValuesMappings), std::end(d_keyValuesMappings), SemanticValue::NoValue);
//
//    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::Backspace)] = SemanticValue::DeletePreviousCharacter;
//    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::DeleteKey)] = SemanticValue::DeleteNextCharacter;
//
//    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::NumpadEnter)] = SemanticValue::Confirm;
//    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::Return)] = SemanticValue::Confirm;
//
//    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::Tab)] = SemanticValue::NavigateToNext;
//
//    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::ArrowLeft)] = SemanticValue::GoToPreviousCharacter;
//    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::ArrowRight)] = SemanticValue::GoToNextCharacter;
//    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::ArrowDown)] = SemanticValue::GoDown;
//    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::ArrowUp)] = SemanticValue::GoUp;
//
//    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::End)] = SemanticValue::GoToEndOfLine;
//    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::Home)] = SemanticValue::GoToStartOfLine;
//    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::PageDown)] = SemanticValue::GoToNextPage;
//    d_keyValuesMappings[static_cast<unsigned char>(Key::Scan::PageUp)] = SemanticValue::GoToPreviousPage;
//
//    SemanticValue value = d_keyValuesMappings[static_cast<unsigned char>(scan_code)];
//
//    // handle combined keys
//    if (ctrl_down && shift_down)
//    {
//        if (scan_code == Key::Scan::ArrowLeft)
//            value = SemanticValue::SelectPreviousWord;
//        else if (scan_code == Key::Scan::ArrowRight)
//            value = SemanticValue::SelectNextWord;
//        else if (scan_code == Key::Scan::End)
//            value = SemanticValue::SelectToEndOfDocument;
//        else if (scan_code == Key::Scan::Home)
//            value = SemanticValue::SelectToStartOfDocument;
//        else if (scan_code == Key::Scan::Z)
//            value = SemanticValue::Redo;
//    }
//    else if (ctrl_down)
//    {
//        if (scan_code == Key::Scan::ArrowLeft)
//            value = SemanticValue::GoToPreviousWord;
//        else if (scan_code == Key::Scan::ArrowRight)
//            value = SemanticValue::GoToNextWord;
//        else if (scan_code == Key::Scan::End)
//            value = SemanticValue::GoToEndOfDocument;
//        else if (scan_code == Key::Scan::Home)
//            value = SemanticValue::GoToStartOfDocument;
//        else if (scan_code == Key::Scan::A)
//            value = SemanticValue::SelectAll;
//        else if (scan_code == Key::Scan::C)
//            value = SemanticValue::Copy;
//        else if (scan_code == Key::Scan::V)
//            value = SemanticValue::Paste;
//        else if (scan_code == Key::Scan::X)
//            value = SemanticValue::Cut;
//        else if (scan_code == Key::Scan::Tab)
//            value = SemanticValue::NavigateToPrevious;
//        else if (scan_code == Key::Scan::Z)
//            value = SemanticValue::Undo;
//        else if (scan_code == Key::Scan::Y)
//            value = SemanticValue::Redo;
//    }
//    else if (shift_down)
//    {
//        if (scan_code == Key::Scan::ArrowLeft)
//            value = SemanticValue::SelectPreviousCharacter;
//        else if (scan_code == Key::Scan::ArrowRight)
//            value = SemanticValue::SelectNextCharacter;
//        else if (scan_code == Key::Scan::ArrowUp)
//            value = SemanticValue::SelectUp;
//        else if (scan_code == Key::Scan::ArrowDown)
//            value = SemanticValue::SelectDown;
//        else if (scan_code == Key::Scan::End)
//            value = SemanticValue::SelectToEndOfLine;
//        else if (scan_code == Key::Scan::Home)
//            value = SemanticValue::SelectToStartOfLine;
//        else if (scan_code == Key::Scan::PageUp)
//            value = SemanticValue::SelectToPreviousPage;
//        else if (scan_code == Key::Scan::PageDown)
//            value = SemanticValue::SelectToNextPage;
//    }
//    if (alt_down)
//    {
//        if(scan_code == Key::Scan::Backspace)
//            value = SemanticValue::Undo;
//    }
//
//    return value;
//}
//} // End of  CEGUI namespace section
