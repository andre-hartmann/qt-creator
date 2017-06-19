#include <QRegularExpression>
static QList<RowData> readLines(QStringRef patch,
    const QVector<QStringRef> lines = patch.split(newLine);
        QStringRef line = lines.at(i);
        const QChar firstCharacter = line.at(0);
            Diff diffToBeAdded(command, line.mid(1).toString() + newLine);
static QList<ChunkData> readChunks(QStringRef patch,
    const QRegularExpression chunkRegExp(
                // beginning of the line
                "(?:\\n|^)"
                // @@ -leftPos[,leftCount] +rightPos[,rightCount] @@
                "@@ -(\\d+)(?:,\\d+)? \\+(\\d+)(?:,\\d+)? @@"
                // optional hint (e.g. function name)
                "(\\ +[^\\n]*)?"
                // end of line (need to be followed by text line)
                "\\n");
    QRegularExpressionMatch match = chunkRegExp.match(patch);
    if (match.hasMatch() && match.capturedStart() == 0) {
            const int pos = match.capturedStart();
            const int leftStartingPos = match.capturedRef(1).toInt();
            const int rightStartingPos = match.capturedRef(2).toInt();
            const QString contextInfo = match.captured(3);
                QStringRef lines = patch.mid(endOfLastChunk,
                                             pos - endOfLastChunk);
            endOfLastChunk = match.capturedEnd();
            match = chunkRegExp.match(patch, endOfLastChunk);
        } while (match.hasMatch());
            QStringRef lines = patch.mid(endOfLastChunk);
static FileData readDiffHeaderAndChunks(QStringRef headerAndChunks,
    QStringRef patch = headerAndChunks;
    const QRegularExpression leftFileRegExp(
          "(?:\\n|^)-{3} "       // "--- "
          "([^\\t\\n]+)"         // "fileName1"
          "(?:\\t[^\\n]*)*\\n"); // optionally followed by: \t anything \t anything ...)
    const QRegularExpression rightFileRegExp(
          "^\\+{3} "             // "+++ "
          "([^\\t\\n]+)"         // "fileName2"
          "(?:\\t[^\\n]*)*\\n"); // optionally followed by: \t anything \t anything ...)
    const QRegularExpression binaryRegExp(
          "^Binary files ([^\\t\\n]+) and ([^\\t\\n]+) differ$");

    // followed either by leftFileRegExp
    const QRegularExpressionMatch leftMatch = leftFileRegExp.match(patch);
    if (leftMatch.hasMatch() && leftMatch.capturedStart() == 0) {
        patch = patch.mid(leftMatch.capturedEnd());
        fileData.leftFileInfo.fileName = leftMatch.captured(1);
        const QRegularExpressionMatch rightMatch = rightFileRegExp.match(patch);
        if (rightMatch.hasMatch() && rightMatch.capturedStart() == 0) {
            patch = patch.mid(rightMatch.capturedEnd());
            fileData.rightFileInfo.fileName = rightMatch.captured(1);
    } else {
        // or by binaryRegExp
        const QRegularExpressionMatch binaryMatch = binaryRegExp.match(patch);
        if (binaryMatch.hasMatch() && binaryMatch.capturedStart() == 0) {
            fileData.leftFileInfo.fileName = binaryMatch.captured(1);
            fileData.rightFileInfo.fileName = binaryMatch.captured(2);
            fileData.binaryFiles = true;
            readOk = true;
        }
static QList<FileData> readDiffPatch(QStringRef patch,
    const QRegularExpression diffRegExp("(?:\\n|^)"          // new line of the beginning of a patch
                                        "("                  // either
                                        "-{3} "              // ---
                                        "[^\\t\\n]+"         // filename1
                                        "(?:\\t[^\\n]*)*\\n" // optionally followed by: \t anything \t anything ...
                                        "\\+{3} "            // +++
                                        "[^\\t\\n]+"         // filename2
                                        "(?:\\t[^\\n]*)*\\n" // optionally followed by: \t anything \t anything ...
                                        "|"                  // or
                                        "Binary files "
                                        "[^\\t\\n]+"         // filename1
                                        " and "
                                        "[^\\t\\n]+"         // filename2
                                        " differ"
                                        ")");                // end of or
    QRegularExpressionMatch diffMatch = diffRegExp.match(patch);
    if (diffMatch.hasMatch()) {
            int pos = diffMatch.capturedStart();
                QStringRef headerAndChunks = patch.mid(lastPos,
                                                       pos - lastPos);
            pos = diffMatch.capturedEnd();
            diffMatch = diffRegExp.match(patch, pos);
        } while (diffMatch.hasMatch());
        if (readOk) {
            QStringRef headerAndChunks = patch.mid(lastPos,
                                                   patch.count() - lastPos - 1);
static FileData readGitHeaderAndChunks(QStringRef headerAndChunks,
    QStringRef patch = headerAndChunks;
    // new file mode octal
    const QRegularExpression newFileMode("^new file mode \\d+\\n");
    // deleted file mode octal
    const QRegularExpression deletedFileMode("^deleted file mode \\d+\\n");
    const QRegularExpression modeChangeRegExp("^old mode \\d+\\nnew mode \\d+\\n");
    // index cap1..cap2(optionally: octal)
    const QRegularExpression indexRegExp("^index (\\w+)\\.{2}(\\w+)(?: \\d+)?(\\n|$)");
    const QRegularExpressionMatch newFileMatch = newFileMode.match(patch);
    if (newFileMatch.hasMatch() && newFileMatch.capturedStart() == 0) {
        patch = patch.mid(newFileMatch.capturedEnd());
    } else {
        const QRegularExpressionMatch deletedFileMatch = deletedFileMode.match(patch);
        if (deletedFileMatch.hasMatch() && deletedFileMatch.capturedStart() == 0) {
            fileData.fileOperation = FileData::DeleteFile;
            rightFileName = devNull;
            patch = patch.mid(deletedFileMatch.capturedEnd());
        } else {
            const QRegularExpressionMatch modeChangeMatch = modeChangeRegExp.match(patch);
            if (modeChangeMatch.hasMatch() && modeChangeMatch.capturedStart() == 0) {
                patch = patch.mid(modeChangeMatch.capturedEnd());
            }
        }
    const QRegularExpressionMatch indexMatch = indexRegExp.match(patch);
    if (indexMatch.hasMatch() && indexMatch.capturedStart() == 0) {
        fileData.leftFileInfo.typeInfo = indexMatch.captured(1);
        fileData.rightFileInfo.typeInfo = indexMatch.captured(2);
        patch = patch.mid(indexMatch.capturedEnd());
        patch = patch.mid(patch.indexOf(QLatin1Char('\n'), leftStart.count()) + 1);
            patch = patch.mid(patch.indexOf(QLatin1Char('\n'), rightStart.count()) + 1);
static FileData readCopyRenameChunks(QStringRef copyRenameChunks,
    QStringRef patch = copyRenameChunks;
    // index cap1..cap2(optionally: octal)
    const QRegularExpression indexRegExp("^index (\\w+)\\.{2}(\\w+)(?: \\d+)?(\\n|$)");
        const QRegularExpressionMatch indexMatch = indexRegExp.match(patch);
        if (indexMatch.hasMatch() && indexMatch.capturedStart() == 0) {
            fileData.leftFileInfo.typeInfo = indexMatch.captured(1);
            fileData.rightFileInfo.typeInfo = indexMatch.captured(2);
            patch = patch.mid(indexMatch.capturedEnd());
                patch = patch.mid(patch.indexOf(QLatin1Char('\n'), leftStart.count()) + 1);
                    patch = patch.mid(patch.indexOf(QLatin1Char('\n'), rightStart.count()) + 1);
static QList<FileData> readGitPatch(QStringRef patch, bool *ok)
    const QRegularExpression simpleGitRegExp(
          "^diff --git a/([^\\n]+) b/\\1\\n" // diff --git a/cap1 b/cap1
          , QRegularExpression::MultilineOption);

    const QRegularExpression similarityRegExp(
          "^diff --git a/([^\\n]+) b/([^\\n]+)\\n" // diff --git a/cap1 b/cap2
          "(?:dis)?similarity index \\d{1,3}%\\n"  // similarity / dissimilarity index xxx% (100% max)
          "(copy|rename) from \\1\\n"              // copy / rename from cap1
          "\\3 to \\2\\n"                          // copy / rename (cap3) to cap2
          , QRegularExpression::MultilineOption);
    QRegularExpressionMatch simpleGitMatch = simpleGitRegExp.match(patch);
    QRegularExpressionMatch similarityMatch = similarityRegExp.match(patch);
        if (pos > 0) { // don't advance in the initial call
            if (simpleGitMatch.hasMatch() && similarityMatch.hasMatch()) {
                const int simpleGitPos = simpleGitMatch.capturedStart();
                const int similarityPos = similarityMatch.capturedStart();
                if (simpleGitPos <= similarityPos)
                    simpleGitMatch = simpleGitRegExp.match(patch, simpleGitMatch.capturedEnd() - 1); // advance only simpleGit
                else
                    similarityMatch = similarityRegExp.match(patch, similarityMatch.capturedEnd() - 1); // advance only similarity
            } else if (simpleGitMatch.hasMatch()) {
                simpleGitMatch = simpleGitRegExp.match(patch, simpleGitMatch.capturedEnd() - 1);
            } else if (similarityMatch.hasMatch()) {
                similarityMatch = similarityRegExp.match(patch, similarityMatch.capturedEnd() - 1);
            }
        }

        if (simpleGitMatch.hasMatch() && similarityMatch.hasMatch()) {
            const int simpleGitPos = simpleGitMatch.capturedStart();
            const int similarityPos = similarityMatch.capturedStart();
            pos = qMin(simpleGitPos, similarityPos);
            simpleGitMatched = (pos == simpleGitPos);
        }

        if (simpleGitMatch.hasMatch()) {
            pos = simpleGitMatch.capturedStart();

        if (similarityMatch.hasMatch()) {
            pos = similarityMatch.capturedStart();
            simpleGitMatched = false;
            return;
        }

        pos = -1;
        simpleGitMatched = true;

        auto collectFileData = [&]() {
            if (endOfLastHeader > 0 && readOk) {
                const int end = pos < 0 ? patch.count() : pos;
                QStringRef headerAndChunks = patch.mid(endOfLastHeader,
                                                       qMax(end - endOfLastHeader - 1, 0));
                if (readOk)
                    fileDataList.append(fileData);
        };

        do {
            collectFileData();
            if (!readOk)
                break;
                const QString fileName = simpleGitMatch.captured(1);
                pos = simpleGitMatch.capturedEnd();
                lastLeftFileName = similarityMatch.captured(1);
                lastRightFileName = similarityMatch.captured(2);
                const QString operation = similarityMatch.captured(3);
                pos = similarityMatch.capturedEnd();
            endOfLastHeader = pos;
        if (readOk)
            collectFileData();
    QStringRef croppedPatch(&patch);
    const QRegularExpression formatPatchEndingRegExp("(\\n-- \\n\\S*\\n\\n$)");
    const QRegularExpressionMatch match = formatPatchEndingRegExp.match(croppedPatch);
    if (match.hasMatch())
        croppedPatch = croppedPatch.left(match.capturedStart() + 1);