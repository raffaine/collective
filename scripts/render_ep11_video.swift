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

// 36 unique eye-level walking scenes across Episode 11 (Season 1 Grand Finale: MVS Retrospective & The Historical Rupture)
let scenes: [SceneSpec] = [
    // Part I: The Minimum Viable Society (MVS vs MVP) — From Exhaustion to Total Dissociation Across Waves A–F
    SceneSpec(path: "doc/podcast/renders/ep01/scenes/scene_01.png", actLabel: "PART I // PREFACE: THE PHYSICS OF EXHAUSTION", placeLabel: "Leaving Atomized Precarity & Capitalist Realism"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_01.png", actLabel: "PART I // WAVE A: AUTOPOIETIC SELF-HOSTING", placeLabel: "20-Year Salvaged Workstation & Nutrition Labels"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_06.png", actLabel: "PART I // WAVE A: INCLUSIVE STEWARD GENESIS", placeLabel: "3-of-5 Shamir Recovery Over Tea (Scenario Tau)"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_11.png", actLabel: "PART I // WAVE A: SOMATIC HOMEOSTASIS", placeLabel: "Wooden-and-Brass Bedroom Sanctuary Rest Latch"),
    SceneSpec(path: "doc/podcast/renders/ep05/scenes/scene_04.png", actLabel: "PART I // WAVE B: 10,000 BP HEARTH BASELINE", placeLabel: "Unconditional Pea Soup & 74°C Pasteurization"),
    SceneSpec(path: "doc/podcast/renders/ep05/scenes/scene_06.png", actLabel: "PART I // WAVE B: CONVIVIAL FABRICATION", placeLabel: "Black-Locust CNC Joinery & <10ms LOTO Safety"),
    SceneSpec(path: "doc/podcast/renders/ep05/scenes/scene_11.png", actLabel: "PART I // WAVE B: FIVE-ARTERY LOGISTICS", placeLabel: "Cargo E-Trike Milk-Runs & DTN Sneakernet"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_01.png", actLabel: "PART I // WAVE C: 90-DAY POSTPARTUM COCOON", placeLabel: "One-Sign Care DAG & Camera-Free Play Pods"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_04.png", actLabel: "PART I // WAVE C: CHOSEN KIN & <500MS EXIT", placeLabel: "Wisteria Covenant Pergola & Safe-Harbor Refuge"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_12.png", actLabel: "PART I // WAVE C: R0_INTERIOR_DARK SHELTER", placeLabel: "Zero-Rent CLT Home & Screenless Brass Toggles"),
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_02.png", actLabel: "PART I // WAVE D: MOTHER-TREE SANCTUARY VETO", placeLabel: "Rotational Winter Coppice & Living Forest Floor"),
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_06.png", actLabel: "PART I // WAVE D: SALVAGE BEFORE MELT", placeLabel: "98% Urban Unbuilding & Solar Induction Foundry"),
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_10.png", actLabel: "PART I // WAVE D: OP_PURGE_R0_LINEAGE", placeLabel: "Zero-RAM Bespoke Cobbling & Kigumi Barn-Raising"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_07.png", actLabel: "PART I // WAVE E: UNMEDIATED STEAM CULTURE", placeLabel: "Brass Mic Kill-Switch, Null Results & Swarms"),
    SceneSpec(path: "doc/podcast/renders/ep09/scenes/scene_04.png", actLabel: "PART I // WAVE F: SOVEREIGN CARE & RESILIENCE", placeLabel: "HPLC Open-Insulin, Storm Refuge & Emeritus Sages"),
    SceneSpec(path: "doc/podcast/renders/ep09/scenes/scene_11.png", actLabel: "PART I // NON-STEWARD SANCTUARY (NO NPCS)", placeLabel: "Equal Standing for Children, Elders & Exhausted"),

    // Part II: Chapter 1 ("The Historical Rupture") — Marx, Lenin, Trotsky, China & Tolkien's One Ring
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_02.png", actLabel: "PART II // CHAPTER 1: THE HISTORICAL RUPTURE", placeLabel: "Karl Marx & Healing the Metabolic Rift"),
    SceneSpec(path: "doc/podcast/renders/ep02/scenes/scene_08.png", actLabel: "PART II // 1917: SO CLOSE TO EMANCIPATION", placeLabel: "Why Head-On State Assault Hit a Structural Wall"),
    SceneSpec(path: "doc/podcast/renders/ep09/scenes/scene_07.png", actLabel: "PART II // ARENDT & BOOKCHIN'S WARNING", placeLabel: "Why Vanguard Power Needed a 72-Hour Sunset"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_11.png", actLabel: "PART II // THE CYBERNETIC BOTTLENECK", placeLabel: "Replacing Central Gosplan With Ostrom & Beer VSM"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_08.png", actLabel: "PART II // TOLKIEN'S ONE RING: CAPITALISM", placeLabel: "The Illusion of Wielding the Ring for Good"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_02.png", actLabel: "PART II // THE ONE RING TAKES NO OTHER LORD", placeLabel: "Refusing 996 Burnout & Eye-of-Sauron Surveillance"),
    SceneSpec(path: "doc/podcast/renders/ep05/scenes/scene_02.png", actLabel: "PART II // THE WAY OF THE JOYFUL HOBBITS", placeLabel: "Loving the Hearth, the Orchard & the Unwatched Song"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_05.png", actLabel: "PART II // CARRYING THE RING ON A CHAIN", placeLabel: "Quarantining Fiat Strictly Inside VLAN 40 Carapace"),

    // Part III: The Medieval Centering Arch (Le Cintre) — Using Society as Leverage for Its Own Emancipation
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_13.png", actLabel: "PART III // THE MEDIEVAL CENTERING ARCH", placeLabel: "Timber Scaffolding Supporting the Rising Vault"),
    SceneSpec(path: "doc/podcast/renders/ep08/scenes/scene_01.png", actLabel: "PART III // GUSTAV LANDAUER & FULLER", placeLabel: "Contracting New Relationships Inside the Old Shell"),
    SceneSpec(path: "doc/podcast/renders/ep04/scenes/scene_03.png", actLabel: "PART III // LEVERAGE 1: THE OASIS MIRROR", placeLabel: "Practicing the New World Through Play First"),
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_04.png", actLabel: "PART III // LEVERAGE 2: TIPPING-FEE INVERSION", placeLabel: "Getting Paid by Layer 7 to Harvest Old-Growth Beams"),
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_01.png", actLabel: "PART III // LEVERAGE 3: 70-90% TAX SLASH", placeLabel: "Statutory Forest Easements & CLT Land Locks"),
    SceneSpec(path: "doc/podcast/renders/ep07/scenes/scene_12.png", actLabel: "PART III // LEVERAGE 4: THE RELEASE WEDGES", placeLabel: "Progressive Siphon & $0 Bill Auto-Shutoff"),
    SceneSpec(path: "doc/podcast/renders/ep10/scenes/scene_11.png", actLabel: "PART III // THE KEYSTONE: 100% TAX ENDOWMENT", placeLabel: "Using Yield to Extinguish Tax Foreclosure Forever"),
    SceneSpec(path: "doc/podcast/renders/ep10/scenes/scene_12.png", actLabel: "PART III // CASTING THE RING INTO THE FIRE", placeLabel: "Striking Out the Wedges & Unlinking l6_l7.sock"),

    // Part IV: Season 1 Finale Epilogue — Technology Is Merely the Soil; Human Flourishing Is the Harvest
    SceneSpec(path: "doc/podcast/renders/ep10/scenes/scene_01.png", actLabel: "PART IV // HONEST HOBBIT REALISM", placeLabel: "Tending Everyday Gardens & Peace Hearths Freely"),
    SceneSpec(path: "doc/podcast/renders/ep10/scenes/scene_13.png", actLabel: "PART IV // 15 HOURS A WEEK: THE HARVEST", placeLabel: "Art, Science, Play & Unhurried Human Presence"),
    SceneSpec(path: "doc/podcast/renders/ep10/scenes/scene_14.png", actLabel: "PART IV // SEEDING THE NEXT WATERSHED", placeLabel: "Genesis Seed Kits & Open-Armed Solidarity"),
    SceneSpec(path: "doc/podcast/renders/ep10/scenes/scene_15.png", actLabel: "SEASON 1 FINALE // THE PLANETARY LIGHTHOUSE", placeLabel: "Technology Is the Soil; Human Flourishing Is the Harvest")
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

    let boxRect = CGRect(x: 48, y: 42, width: 780, height: 86)
    let boxPath = CGPath(roundedRect: boxRect, cornerWidth: 8, cornerHeight: 8, transform: nil)
    ctx.setFillColor(CGColor(red: 0.06, green: 0.08, blue: 0.10, alpha: 0.62))
    ctx.addPath(boxPath)
    ctx.fillPath()

    ctx.setStrokeColor(CGColor(red: 0.96, green: 0.77, blue: 0.19, alpha: 0.45))
    ctx.setLineWidth(1.5)
    ctx.addPath(boxPath)
    ctx.strokePath()

    let pillRect = CGRect(x: 48, y: height - 66, width: 700, height: 34)
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

    drawText("RADIO LAMBDA // EP. 11 (FINALE): MINIMUM VIABLE SOCIETY & HISTORICAL RUPTURE", font: fontTop, color: gold, x: 64, y: CGFloat(height - 54))
    drawText(actLabel, font: fontAct, color: green, x: 68, y: 96)
    drawText(placeLabel, font: fontPlace, color: white, x: 68, y: 62)

    return ctx.makeImage()!
}

let semaphore = DispatchSemaphore(value: 0)

Task {
    do {
        let audioPath = "doc/podcast/episodes/11.episode.m4a"
        let finalOutPath = "doc/podcast/episodes/11.episode_youtube.mp4"
        let tempVideoPath = "/tmp/ep11_silent_video.mp4"

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
