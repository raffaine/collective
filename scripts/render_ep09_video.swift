import Foundation
import AVFoundation
import CoreGraphics
import CoreText
import AppKit

struct SceneSpec {
    let path: String
    let actLabel: String
    let placeLabel: String
}

// 30 unique eye-level walking scenes across Episode 09 (Wave F Part 1: Chi, Upsilon, Phi)
let scenes: [SceneSpec] = [
    // Part I: Scenario Chi — Zero-LLM Clinical Triage, Six-Modality Care & >=9950bp Open Pharmacopoeia
    SceneSpec(path: "doc/podcast/renders/ep09/scenes/scene_01.png", actLabel: "PART I // SCENARIO CHI: <500MS ZERO-LLM DISPATCH", placeLabel: "Clinic Vestibule & Red Trauma Pull-Cord"),
    SceneSpec(path: "doc/podcast/renders/ep09/scenes/scene_02.png", actLabel: "PART I // SCENARIO CHI: SIX-MODALITY PRIMARY CARE", placeLabel: "Camera-Free Midwifery & Herb Garden Exam Suite"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_02.png", actLabel: "PART I // SCENARIO CHI: PEDIATRIC & DENTAL COMMONS", placeLabel: "Warm Timber Care Suite & 38°C Safety Valve"),
    SceneSpec(path: "doc/podcast/renders/ep09/scenes/scene_03.png", actLabel: "PART I // SCENARIO CHI: NON-CARCERAL PEER RESPITE", placeLabel: "Unrecorded Sanctuary Room & Bamboo Water Court"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_06.png", actLabel: "PART I // SCENARIO CHI: R0_CLINICAL_ENCLAVE", placeLabel: "Subpoena-Sterile Bodily & Reproductive Sanctuary"),
    SceneSpec(path: "doc/podcast/renders/ep09/scenes/scene_04.png", actLabel: "PART I // SCENARIO CHI: >=9950 BP HPLC/GC-MS GATE", placeLabel: "Open-Insulin Bioreactor & Analytical Wet-Lab"),
    SceneSpec(path: "doc/podcast/renders/ep09/scenes/scene_05.png", actLabel: "PART I // SCENARIO CHI: ASSAY-LOCKED DISPENSARY", placeLabel: "Solenoid Apothecary Cabinet & Cold-Chain Larder"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_09.png", actLabel: "PART I // SCENARIO CHI: BOTANICAL COMPOUNDING", placeLabel: "Dual-Assay Herbal & Essential Medicine Cloister"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_13.png", actLabel: "PART I // SCENARIO CHI: RESPONDER DECOMPRESSION", placeLabel: "Post-Call Medic Hydrotherapy & Hinoki Soak"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_06.png", actLabel: "PART I // SCENARIO CHI: TERTIARY HL7/FHIR BRIDGE", placeLabel: "Chosen-Kin POA Packet & Catastrophic Pool"),

    // Part II: Scenario Upsilon — Ecological Shock Buffering, <16ms Black-Start & Open-Hearth Storm Refuge
    SceneSpec(path: "doc/podcast/renders/ep09/scenes/scene_06.png", actLabel: "PART II // SCENARIO UPSILON: KEYLINE SPONGE SWALES", placeLabel: "Biochar Rainwater Infiltration & Willow Firebreak"),
    SceneSpec(path: "doc/podcast/renders/ep02/scenes/scene_02.png", actLabel: "PART II // SCENARIO UPSILON: PRE-EMPTIVE BUFFERING", placeLabel: "Hydrated Terraced Orchard & Aquifer Recharge"),
    SceneSpec(path: "doc/podcast/renders/ep09/scenes/scene_07.png", actLabel: "PART II // SCENARIO UPSILON: <16MS BLACK-START", placeLabel: "5-Tier Islanding Switchgear & 0-mW E-Ink Slate"),
    SceneSpec(path: "doc/podcast/renders/ep02/scenes/scene_01.png", actLabel: "PART II // SCENARIO UPSILON: RED-LEVER CONTINUITY", placeLabel: "48V LiFePO4 Reserve & Manual Disconnect Bus"),
    SceneSpec(path: "doc/podcast/renders/ep09/scenes/scene_08.png", actLabel: "PART II // SCENARIO UPSILON: OPEN-HEARTH REFUGE", placeLabel: "Amber Storm Lantern on Dark Suburban Street"),
    SceneSpec(path: "doc/podcast/renders/ep09/scenes/scene_09.png", actLabel: "PART II // SCENARIO UPSILON: $0 NEIGHBOR SOLIDARITY", placeLabel: "HEPA Hall, Hot Pea Soup & Free Device Charging"),
    SceneSpec(path: "doc/podcast/renders/ep05/scenes/scene_04.png", actLabel: "PART II // SCENARIO UPSILON: TIER-2 HEARTH CAULDRON", placeLabel: "72-Hour Islanded Soup Stockpot & Warm Kitchen"),
    SceneSpec(path: "doc/podcast/renders/ep05/scenes/scene_12.png", actLabel: "PART II // SCENARIO UPSILON: 14-DAY LORA PAGERS", placeLabel: "915 MHz Reticulum Welfare-Check Mesh"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_13.png", actLabel: "PART II // SCENARIO UPSILON: 72-HOUR SUNSET CLOCK", placeLabel: "Automatic Peacetime Restoration & Valley Bridge"),
    SceneSpec(path: "doc/podcast/renders/ep09/scenes/scene_10.png", actLabel: "PART II // SCENARIO UPSILON: SHOCK-VERIFIED JUBILEE", placeLabel: "Morning After the Storm & Zeroed BCC Ledger"),

    // Part III: Scenario Phi — Dignified Aging, Emeritus Sagehood, 60GHz mmWave Radar & Memorial Groves
    SceneSpec(path: "doc/podcast/renders/ep09/scenes/scene_11.png", actLabel: "PART III // SCENARIO PHI: INTERGENERATIONAL COURT", placeLabel: "Zero-Step Elder Cottages & Children's Play Garden"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_08.png", actLabel: "PART III // SCENARIO PHI: <=20% CARE DISPERSION", placeLabel: "Five-Neighbor Rotational Tea & Garden Stroll"),
    SceneSpec(path: "doc/podcast/renders/ep09/scenes/scene_12.png", actLabel: "PART III // SCENARIO PHI: ZERO-CAMERA 60GHZ RADAR", placeLabel: "Edge RAM Posture Sensor & Brass Tactile Companion"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_12.png", actLabel: "PART III // SCENARIO PHI: DIGNIFIED R0 AUTONOMY", placeLabel: "Screenless Ground-Floor Suite & Warm Oak Floors"),
    SceneSpec(path: "doc/podcast/renders/ep09/scenes/scene_13.png", actLabel: "PART III // SCENARIO PHI: T4 EMERITUS SAGEHOOD", placeLabel: "Zero-Decay Wisdom Archive & 50-Year Flood Maps"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_05.png", actLabel: "PART III // SCENARIO PHI: ORAL HISTORY BINDERY", placeLabel: "Hand-Bound Craft Ledgers & Socratic Mentorship"),
    SceneSpec(path: "doc/podcast/renders/ep09/scenes/scene_14.png", actLabel: "PART III // SCENARIO PHI: UNMEDIATED HOSPICE VIGIL", placeLabel: "Candle-Lit Sanctuary Suite & Brass Vigil Switch"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_04.png", actLabel: "PART III // SCENARIO PHI: POLST & HEIRLOOM ARMOR", placeLabel: "Chosen-Kin Proxy Covenant & Peaceful Garden"),
    SceneSpec(path: "doc/podcast/renders/ep09/scenes/scene_15.png", actLabel: "PART III // SCENARIO PHI: ECOLOGICAL TERRAMATION", placeLabel: "Sacred Chestnut & Oak Memorial Coppice Grove"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_15.png", actLabel: "PART III // WAVE F (PART 1): THE FULL CIRCLE OF CARE", placeLabel: "Twilight Watershed & Permanent Wisdom Anchors")
]

let width = 1920
let height = 1080
let fps: Int32 = 8
let crossfadeSec: Double = 5.0
let colorSpace = CGColorSpaceCreateDeviceRGB()
let bitmapInfo = CGImageAlphaInfo.premultipliedFirst.rawValue | CGBitmapInfo.byteOrder32Little.rawValue

func loadNormalizedCGImage(_ path: String) -> CGImage {
    guard let nsImg = NSImage(contentsOfFile: path),
          let srcCG = nsImg.cgImage(forProposedRect: nil, context: nil, hints: nil) else {
        fatalError("Failed to load image at \(path)")
    }
    guard let ctx = CGContext(
        data: nil,
        width: width,
        height: height,
        bitsPerComponent: 8,
        bytesPerRow: width * 4,
        space: colorSpace,
        bitmapInfo: bitmapInfo
    ) else {
        fatalError("Cannot create normalize CGContext")
    }
    ctx.interpolationQuality = .high
    let scaleX = CGFloat(width) / CGFloat(srcCG.width)
    let scaleY = CGFloat(height) / CGFloat(srcCG.height)
    let scale = max(scaleX, scaleY)
    let drawW = CGFloat(srcCG.width) * scale
    let drawH = CGFloat(srcCG.height) * scale
    let dx = (CGFloat(width) - drawW) * 0.5
    let dy = (CGFloat(height) - drawH) * 0.5
    ctx.draw(srcCG, in: CGRect(x: dx, y: dy, width: drawW, height: drawH))
    return ctx.makeImage()!
}

func makeHudCGImage(actLabel: String, placeLabel: String) -> CGImage {
    guard let ctx = CGContext(
        data: nil,
        width: width,
        height: height,
        bitsPerComponent: 8,
        bytesPerRow: width * 4,
        space: colorSpace,
        bitmapInfo: bitmapInfo
    ) else {
        fatalError("Cannot create HUD CGContext")
    }

    let boxRect = CGRect(x: 48, y: 42, width: 760, height: 86)
    let boxPath = CGPath(roundedRect: boxRect, cornerWidth: 8, cornerHeight: 8, transform: nil)
    ctx.setFillColor(CGColor(red: 0.06, green: 0.08, blue: 0.10, alpha: 0.62))
    ctx.addPath(boxPath)
    ctx.fillPath()

    ctx.setStrokeColor(CGColor(red: 0.96, green: 0.77, blue: 0.19, alpha: 0.45))
    ctx.setLineWidth(1.5)
    ctx.addPath(boxPath)
    ctx.strokePath()

    let pillRect = CGRect(x: 48, y: height - 66, width: 660, height: 34)
    let pillPath = CGPath(roundedRect: pillRect, cornerWidth: 6, cornerHeight: 6, transform: nil)
    ctx.setFillColor(CGColor(red: 0.06, green: 0.08, blue: 0.10, alpha: 0.55))
    ctx.addPath(pillPath)
    ctx.fillPath()

    func drawText(_ text: String, font: CTFont, color: CGColor, x: CGFloat, y: CGFloat) {
        let attrs: [NSAttributedString.Key: Any] = [
            NSAttributedString.Key(kCTFontAttributeName as String): font,
            NSAttributedString.Key(kCTForegroundColorAttributeName as String): color
        ]
        let attrStr = NSAttributedString(string: text, attributes: attrs)
        let line = CTLineCreateWithAttributedString(attrStr)
        ctx.textPosition = CGPoint(x: x, y: y)
        CTLineDraw(line, ctx)
    }

    let fontTop = CTFontCreateWithName("Courier-Bold" as CFString, 15, nil)
    let fontAct = CTFontCreateWithName("Courier-Bold" as CFString, 16, nil)
    let fontPlace = CTFontCreateWithName("Helvetica-Bold" as CFString, 23, nil)

    let gold = CGColor(red: 0.96, green: 0.77, blue: 0.19, alpha: 0.95)
    let green = CGColor(red: 0.48, green: 0.82, blue: 0.42, alpha: 0.95)
    let white = CGColor(red: 0.95, green: 0.96, blue: 0.98, alpha: 0.96)

    drawText("RADIO LAMBDA // EP. 09: WAVE F (PART 1) — CHI, UPSILON & PHI", font: fontTop, color: gold, x: 64, y: CGFloat(height - 54))
    drawText(actLabel, font: fontAct, color: green, x: 68, y: 96)
    drawText(placeLabel, font: fontPlace, color: white, x: 68, y: 62)

    return ctx.makeImage()!
}

let semaphore = DispatchSemaphore(value: 0)

Task {
    do {
        let audioPath = "doc/podcast/episodes/09.episode.m4a"
        let finalOutPath = "doc/podcast/episodes/09.episode_youtube.mp4"
        let tempVideoPath = "/tmp/ep09_silent_video.mp4"

        try? FileManager.default.removeItem(atPath: tempVideoPath)
        try? FileManager.default.removeItem(atPath: finalOutPath)

        let audioURL = URL(fileURLWithPath: audioPath)
        let audioAsset = AVURLAsset(url: audioURL)
        let durationCM = try await audioAsset.load(.duration)
        let durationSec = CMTimeGetSeconds(durationCM)
        print(String(format: "Audio duration: %.2f seconds (%.2f minutes)", durationSec, durationSec / 60.0))

        let cgScenes = scenes.map { loadNormalizedCGImage($0.path) }
        let hudScenes = scenes.map { makeHudCGImage(actLabel: $0.actLabel, placeLabel: $0.placeLabel) }
        let sceneDuration = durationSec / Double(scenes.count)

        let writer = try AVAssetWriter(outputURL: URL(fileURLWithPath: tempVideoPath), fileType: .mp4)
        let videoSettings: [String: Any] = [
            AVVideoCodecKey: AVVideoCodecType.h264,
            AVVideoWidthKey: width,
            AVVideoHeightKey: height,
            AVVideoCompressionPropertiesKey: [
                AVVideoAverageBitRateKey: 1_800_000,
                AVVideoMaxKeyFrameIntervalKey: 16,
                AVVideoProfileLevelKey: AVVideoProfileLevelH264HighAutoLevel
            ]
        ]
        let writerInput = AVAssetWriterInput(mediaType: .video, outputSettings: videoSettings)
        writerInput.expectsMediaDataInRealTime = false

        let pixelBufferAttrs: [String: Any] = [
            kCVPixelBufferPixelFormatTypeKey as String: Int(kCVPixelFormatType_32BGRA),
            kCVPixelBufferWidthKey as String: width,
            kCVPixelBufferHeightKey as String: height,
            kCVPixelBufferCGImageCompatibilityKey as String: true,
            kCVPixelBufferCGBitmapContextCompatibilityKey as String: true
        ]
        let adaptor = AVAssetWriterInputPixelBufferAdaptor(
            assetWriterInput: writerInput,
            sourcePixelBufferAttributes: pixelBufferAttrs
        )

        writer.add(writerInput)
        writer.startWriting()
        writer.startSession(atSourceTime: .zero)

        let totalFrames = Int(ceil(durationSec * Double(fps)))
        print("Rendering \(totalFrames) frames at \(fps) fps across \(scenes.count) scenes...")

        func drawSceneKenBurns(_ ctx: CGContext, img: CGImage, index: Int, progress: Double, alpha: CGFloat) {
            let p = CGFloat(max(0.0, min(1.0, progress)))
            let zoomIn = (index % 2 == 0)
            let scale: CGFloat = zoomIn ? (1.02 + 0.08 * p) : (1.10 - 0.08 * p)
            let panDir: CGFloat = (index % 3 == 0) ? 1.0 : ((index % 3 == 1) ? -1.0 : 0.4)
            let maxPanX = CGFloat(width) * 0.025
            let maxPanY = CGFloat(height) * 0.015
            let offsetX = panDir * maxPanX * (p - 0.5)
            let offsetY = -panDir * maxPanY * (p - 0.5)

            let drawW = CGFloat(width) * scale
            let drawH = CGFloat(height) * scale
            let x = (CGFloat(width) - drawW) * 0.5 + offsetX
            let y = (CGFloat(height) - drawH) * 0.5 + offsetY

            ctx.saveGState()
            if alpha < 0.999 {
                ctx.setAlpha(alpha)
            }
            ctx.interpolationQuality = .low
            ctx.draw(img, in: CGRect(x: x, y: y, width: drawW, height: drawH))
            ctx.restoreGState()
        }

        for frameIdx in 0..<totalFrames {
            while !writerInput.isReadyForMoreMediaData {
                Thread.sleep(forTimeInterval: 0.002)
            }

            autoreleasepool {
                let t = Double(frameIdx) / Double(fps)
                let rawIdx = Int(t / sceneDuration)
                let curIdx = min( max(0, rawIdx), scenes.count - 1 )
                let sceneLocalT = t - Double(curIdx) * sceneDuration
                let prog = sceneLocalT / sceneDuration

                var pixelBufferOut: CVPixelBuffer?
                CVPixelBufferPoolCreatePixelBuffer(nil, adaptor.pixelBufferPool!, &pixelBufferOut)
                guard let pixelBuffer = pixelBufferOut else { return }

                CVPixelBufferLockBaseAddress(pixelBuffer, [])
                let baseAddr = CVPixelBufferGetBaseAddress(pixelBuffer)!
                let bytesPerRow = CVPixelBufferGetBytesPerRow(pixelBuffer)

                let ctx = CGContext(
                    data: baseAddr,
                    width: width,
                    height: height,
                    bitsPerComponent: 8,
                    bytesPerRow: bytesPerRow,
                    space: colorSpace,
                    bitmapInfo: bitmapInfo
                )!

                drawSceneKenBurns(ctx, img: cgScenes[curIdx], index: curIdx, progress: prog, alpha: 1.0)

                if curIdx + 1 < scenes.count && (sceneDuration - sceneLocalT) < crossfadeSec {
                    let fadeAlpha = CGFloat((crossfadeSec - (sceneDuration - sceneLocalT)) / crossfadeSec)
                    drawSceneKenBurns(ctx, img: cgScenes[curIdx + 1], index: curIdx + 1, progress: 0.0, alpha: fadeAlpha)
                }

                ctx.interpolationQuality = .none
                ctx.draw(hudScenes[curIdx], in: CGRect(x: 0, y: 0, width: width, height: height))

                let totalProg = CGFloat(t / durationSec)
                ctx.setFillColor(CGColor(red: 1.0, green: 1.0, blue: 1.0, alpha: 0.18))
                ctx.fill(CGRect(x: 0, y: 0, width: CGFloat(width), height: 6))
                ctx.setFillColor(CGColor(red: 0.96, green: 0.77, blue: 0.19, alpha: 0.92))
                ctx.fill(CGRect(x: 0, y: 0, width: CGFloat(width) * totalProg, height: 6))

                CVPixelBufferUnlockBaseAddress(pixelBuffer, [])

                let pts = CMTime(value: CMTimeValue(frameIdx), timescale: fps)
                adaptor.append(pixelBuffer, withPresentationTime: pts)
            }

            if frameIdx % 1920 == 0 && frameIdx > 0 {
                let pct = Int((Double(frameIdx) / Double(totalFrames)) * 100.0)
                print("Progress: \(pct)% (\(frameIdx)/\(totalFrames) frames)")
            }
        }

        writerInput.markAsFinished()
        await writer.finishWriting()
        print("Silent video track rendered. Now muxing audio losslessly...")

        let composition = AVMutableComposition()
        let videoAsset = AVURLAsset(url: URL(fileURLWithPath: tempVideoPath))

        let compVideoTrack = composition.addMutableTrack(withMediaType: .video, preferredTrackID: kCMPersistentTrackID_Invalid)!
        let srcVideoTrack = try await videoAsset.loadTracks(withMediaType: .video).first!
        try compVideoTrack.insertTimeRange(CMTimeRange(start: .zero, duration: durationCM), of: srcVideoTrack, at: .zero)

        let compAudioTrack = composition.addMutableTrack(withMediaType: .audio, preferredTrackID: kCMPersistentTrackID_Invalid)!
        let srcAudioTrack = try await audioAsset.loadTracks(withMediaType: .audio).first!
        try compAudioTrack.insertTimeRange(CMTimeRange(start: .zero, duration: durationCM), of: srcAudioTrack, at: .zero)

        let exporter = AVAssetExportSession(asset: composition, presetName: AVAssetExportPresetPassthrough)!
        exporter.outputURL = URL(fileURLWithPath: finalOutPath)
        exporter.outputFileType = .mp4
        await exporter.export()

        try? FileManager.default.removeItem(atPath: tempVideoPath)
        print("SUCCESS: Created YouTube video at \(finalOutPath)")
        semaphore.signal()
    } catch {
        print("ERROR: \(error)")
        exit(1)
    }
}

semaphore.wait()
